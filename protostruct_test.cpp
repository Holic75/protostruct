#include "protostruct.h"

#include <iostream>
#include <string>

namespace detail
{
	template<class T>
	std::string protostructEntryToString(const protostruct::ProtoStructEntry<protostruct::field_types::repeated, T>& p, int& identation)
	{
		std::string out = "";
		for (int i = 0; i < p.size(); i++)
		{
			out += "\n" + std::string(identation * 2, ' ') + "[" + std::to_string(i) + "]" + " " + protostructEntryToStringConverter<false>::convert(p.data()[i].get(), identation);
		}
		return out;
	}


	template<class T>
	std::string protostructEntryToString(const protostruct::ProtoStructEntry<protostruct::field_types::required, T>& p, int& identation)
	{
		std::string s = protostructEntryToStringConverter<false>::convert(p.get(), identation);
		return s;
	}


	template<class T>
	std::string protostructEntryToString(const protostruct::ProtoStructEntry<protostruct::field_types::optional, T>& p, int& identation)
	{
		if (p.isSet())
		{	
			return protostructEntryToStringConverter<false>::convert(p.get(), identation);
		}
		return "";
	}


	template<class T, size_t N>
	std::string protostructEntryToString(const protostruct::ProtoStructEntry<protostruct::field_types::repeated_fixed<N>, T>& p, int& identation)
	{
		std::string out = "";
		for (int i = 0; i < p.size(); i++)
		{
			out += "\n" + std::string(identation * 2, ' ') + "[" + std::to_string(i) + "]" + " " + protostructEntryToStringConverter<false>::convert(p.data()[i].get(), identation);
		}
		return out;
	}



	template <bool P = false>
	struct protostructEntryToStringConverter
	{
		template<size_t id = 1, class T>
		static typename std::enable_if<std::is_base_of<protostruct::ProtoenumBase, T>::value, std::string>::type
			convert(const T& p, int& identation)
		{
			auto name = T::findNameByValue(p.get());
			if (name != nullptr)
			{
				return std::string(name) + " (" + std::to_string(p.get()) +")";
			}
			else
			{
				return std::to_string(p.get());
			}
		}


		template<size_t id = 1, class T>
		static typename std::enable_if<std::is_base_of<protostruct::ProtounionBase, T>::value, std::string>::type
			convert(const T& p, int& identation)
		{
			std::string field_name = T::template proto_entry<id>::name;
			if (T::template proto_entry<id>::hasField(&p))
			{
				identation++;
				std::string out = "\n" + std::string(identation * 2, ' ') + field_name + " " + protostructEntryToString(T::template proto_entry<id>::get(&p), identation);
				identation--;
				return out;
			}
			return protostructEntryToStringConverter<T::template proto_entry<id + 1>::is_end>::convert<id + 1>(p, identation);
		}


		template<size_t id = 1, class T>
		static typename std::enable_if<!std::is_arithmetic<T>::value 
									   && !std::is_base_of<protostruct::ProtoenumBase, T>::value
			                           && !std::is_base_of<protostruct::ProtounionBase, T>::value, std::string>::type
		convert(const T& p, int& identation)
		{
			std::string field_name = T::template proto_entry<id>::name;
			identation++;
		    std::string out = "\n" + std::string(identation * 2, ' ') + field_name + " " + protostructEntryToString(T::template proto_entry<id>::get(&p), identation);
			identation--;
			return out + protostructEntryToStringConverter<T::template proto_entry<id + 1>::is_end>::convert<id + 1>(p, identation);
		}

		template<size_t id = 1, class T>
		static typename std::enable_if<std::is_arithmetic<T>::value, std::string>::type
		convert(T p, int& identation)
		{
			return std::to_string(p);
		}
	};



	template <>
	struct protostructEntryToStringConverter<true>
	{
		template<size_t id = 1, class T>
		static std::string convert(const T& p, int identation)
		{
			return "" ;
		}
	};
}



template <class  T>
typename std::enable_if<std::is_base_of<protostruct::ProtostructBase, T>::value, std::string>::type
protostructEntryToString(const T& p)
{
	int identation = 0;
	return std::string(p.name) + detail::protostructEntryToStringConverter<false>::convert(p, identation);
}

__PROTOSTRUCT_BEGIN(MyMessage0)
__PROTOSTRUCT_ENTRY(required, int16_t, my_int0, 1)
__PROTOSTRUCT_ENTRY(required, float, my_float0, 2)
__PROTOSTRUCT_END

__PROTOUNION_BEGIN(MyUnion)
__PROTOUNION_ENTRY(optional, float, union_float, 1)
__PROTOUNION_ENTRY(optional, uint16_t, union_uint, 2)
__PROTOUNION_ENTRY(repeated_fixed<5>, int8_t, union_int_array, 3)
__PROTOUNION_ENTRY(repeated_fixed<2>, MyMessage0, union_message_array, 4)
__PROTOUNION_END


__PROTOENUM_BEGIN(MyEnum, uint8_t)
__PROTOENUM_ENTRY(MyEnumValue1, 0, 1)
__PROTOENUM_ENTRY(MyEnumValue2, 1, 2)
__PROTOENUM_ENTRY(MyEnumValue3, 2, 3)
__PROTOENUM_END


__PROTOSTRUCT_BEGIN(MyMessage1)
__PROTOSTRUCT_ENTRY(required, uint32_t, my_int1, 1)
__PROTOSTRUCT_ENTRY(required, float, my_float1, 2)
__PROTOSTRUCT_ENTRY(required, MyEnum, my_enum1, 3)
__PROTOSTRUCT_ENTRY(optional, MyUnion, my_union, 4)
__PROTOSTRUCT_END

__PROTOSTRUCT_BEGIN(MyMessage2)
__PROTOSTRUCT_ENTRY(repeated, MyMessage1, my_msg_array, 1)
__PROTOSTRUCT_END


__PROTOSTRUCT_BEGIN(MyMessage3)
__PROTOSTRUCT_ENTRY(optional, MyMessage1, my_msg1, 1);
__PROTOSTRUCT_ENTRY(repeated_fixed<4>, float, my_float_array, 2)
__PROTOSTRUCT_ENTRY(optional, uint16_t, my_int2, 3)
__PROTOSTRUCT_ENTRY(required, uint16_t, my_int3, 4)
__PROTOSTRUCT_END




int main()
{
	MyMessage1 a1;
	a1.my_int1() = 10;
	a1.my_float1() = 11.5;
	a1.my_enum1() = MyEnum::MyEnumValue2;
	a1.my_union().ref().union_float() = 0.5f;

	MyMessage1 a2;
	a2.my_int1() = 20;
	a2.my_float1() = 30.5;
	a2.my_enum1() = MyEnum::MyEnumValue1;
	a2.my_union().ref().union_uint() = 10;

	MyMessage1* my_msg_buffer = new MyMessage1[4];
	MyMessage2 array_a;

	array_a.my_msg_array().set_buffer(my_msg_buffer);
	array_a.my_msg_array().set_size(4);
	array_a.my_msg_array().data()[0] = a1;
	array_a.my_msg_array().data()[1] = a2;
	array_a.my_msg_array().data()[2] = a1;
	array_a.my_msg_array().data()[2].ref().my_int1() = 100;
	array_a.my_msg_array().data()[3] = a2;
	auto& my_union = array_a.my_msg_array().data()[2].ref().my_union().ref();
	for (int i = 0; i < my_union.union_int_array().size(); i++)
	{
		my_union.union_int_array().data()[i] = i - 2;
	}

	auto& my_union2 = array_a.my_msg_array().data()[3].ref().my_union().ref();
	MyMessage0 msg00, msg01;
	msg00.my_float0() = -0.5f;
	msg00.my_int0() = 23;

	msg01.my_float0() = 2.5f;
	msg01.my_int0() = -33;

	my_union2.union_message_array().data()[0] = msg00;
	my_union2.union_message_array().data()[1] = msg01;


	
	MyMessage3 b;
	MyMessage3 c;

	b.my_msg1() = a1;
	for (int i = 0; i < 4; i++)
	{
		b.my_float_array().data()[i] = i * 0.5;
	}


	void* buf = new uint8_t[100];

	std::cout << "MyUnion entry size: " << MyUnion::MAX_DATA_SIZE << std::endl;
	std::cout << protostructEntryToString(array_a) << std::endl;

	b.encode(buf);
	c.decode(buf, 100);
	
	c.my_int3() = 20;

	std::cout << protostructEntryToString(c) << std::endl;
}