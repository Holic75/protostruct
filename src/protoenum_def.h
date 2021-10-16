#ifndef PROTOENUM_DEF_H
#define PROTOENUM_DEF_H

namespace protostruct
{
	class ProtoenumBase
	{

	};
}

#define __PROTOENUM_BEGIN(NAME, TYPE)                                                                                     \
class NAME: protostruct::ProtoenumBase                                                                                    \
{                                                                                                                         \
public:                                                                                                                   \
    typedef TYPE enum_type;                                                                                               \
    static constexpr const char* name = #NAME;                                                                            \
    NAME(){enum_value_ = proto_entry<1>::val;};                                                                           \
    NAME(const enum_type& val) {enum_value_ = val;};                                                                      \
private:                                                                                                                  \
    protostruct::ProtoStructEntry<protostruct::field_types::required, enum_type> enum_value_;                             \
public:                                                                                                                   \
    typedef NAME class_type;                                                                                              \
    template<size_t id, bool dummy = true>                                                                                \
    struct proto_entry                                                                                                    \
    {                                                                                                                     \
        static constexpr const char* name = "";                                                                           \
        static constexpr bool is_end = true;                                                                              \
    };                                                                                                                    \
private:                                                                                                                  \
    template<size_t id, bool dummy = true>                                                                                \
    struct __proto_helper                                                                                                 \
    {                                                                                                                     \
        static const char* nameByValue(const enum_type& val)  {return nullptr;};                                          \
        static bool valueByName(const char*, enum_type&)  {return false;};                                                \
        static bool hasName(const char*)  {return false;};                                                                \
        static bool hasValue(const enum_type&)  {return false;};                                                          \
        static constexpr size_t count()                                                                                   \
        {                                                                                                                 \
            return 0;                                                                                                     \
        }                                                                                                                 \
    };                                                                                                                                                                                                                 

#define __PROTOENUM_ENTRY(NAME, VALUE, ID)                                                                                \
public:                                                                                                                   \
    static constexpr const enum_type NAME = VALUE;                                                                        \
                                                                                                                          \
    template<bool dummy>                                                                                                  \
    struct proto_entry<ID, dummy>                                                                                         \
    {                                                                                                                     \
        static constexpr bool is_end = false;                                                                             \
        static constexpr const char* name = #NAME;                                                                        \
        static constexpr const enum_type val = VALUE;                                                                     \
    };                                                                                                                    \
private:                                                                                                                  \
    template<bool dummy>                                                                                                  \
    struct __proto_helper<ID, dummy>                                                                                      \
    {                                                                                                                     \
        static const char* nameByValue( const enum_type& val)                                                             \
        {                                                                                                                 \
            if (val == class_type::NAME)                                                                                  \
            {                                                                                                             \
                return proto_entry<ID>::name;                                                                             \
            }                                                                                                             \
            return __proto_helper<ID+1>::nameByValue(val);                                                                \
        };                                                                                                                \
        static bool valueByName(const char* enum_name, enum_type& val)                                                    \
        {                                                                                                                 \
            if (enum_name == proto_entry<ID>::name)                                                                       \
            {                                                                                                             \
                val = class_type::NAME;                                                                                   \
                return true;                                                                                              \
            }                                                                                                             \
            return __proto_helper<ID+1>::valueByName(enum_name, val);                                                     \
        }                                                                                                                 \
        static bool hasName(const char* enum_name)                                                                        \
        {                                                                                                                 \
            return (enum_name == proto_entry<ID>::name) ? true : __proto_helper<ID+1>::hasName(enum_name);                \
        }                                                                                                                 \
        static bool hasValue(const enum_type& val)                                                                        \
        {                                                                                                                 \
             return (val == class_type::NAME) ? true : __proto_helper<ID+1>::hasValue(val);                               \
        }                                                                                                                 \
                                                                                                                          \
        static constexpr size_t count()                                                                                   \
        {   static_assert(ID == 1 || !proto_entry<ID-1>::is_end,                                                          \
            "Not all protoenum entries were defined");                                                                    \
            return 1 + __proto_helper<ID+1>::count();                                                                     \
        }                                                                                                                 \
    };                                                                                                                    \

#define __PROTOENUM_END                                                                                                   \
public:                                                                                                                   \
    size_t encode(void* buffer) const                                                                                     \
    {                                                                                                                     \
        return enum_value_.encode(buffer);                                                                                \
    }                                                                                                                     \
    size_t decode(const void* buffer, size_t length)                                                                      \
    {                                                                                                                     \
        return enum_value_.decode(buffer, length);                                                                        \
    }                                                                                                                     \
    size_t encodedSize() const {return enum_value_.encodedSize();};                                                       \
    static constexpr  size_t fieldCount(){ return __proto_helper<1>::count();};                                           \
    static const char* findNameByValue(const enum_type& val)  {return __proto_helper<1>::nameByValue(val);};              \
    static bool findValueByName(const char* enum_name, enum_type& val)                                                    \
    {return __proto_helper<1>::valueByName(enum_name, val);};                                                             \
    static bool hasName(const char* enum_name)  {return __proto_helper<1>::hasName(enum_name);};                          \
    static bool hasValue(const enum_type& val)  {return __proto_helper<1>::hasValue(val);};                               \
    class_type& operator=(const enum_type& val)                                                                           \
    {                                                                                                                     \
        enum_value_ = val;                                                                                                \
        return *this;                                                                                                     \
    }                                                                                                                     \
    operator enum_type() {return enum_value_.get();};                                                                     \
    enum_type get() const {return enum_value_.get();};                                                                    \
};

#endif