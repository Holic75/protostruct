#ifndef PROTOUNION_DEF_H
#define PROTOUNION_DEF_H


namespace protostruct
{
	class ProtounionBase
	{

	};
}

#define __PROTOUNION_BEGIN(NAME)                                                                                          \
class NAME: protostruct::ProtounionBase                                                                                   \
{                                                                                                                         \
public:                                                                                                                   \
    NAME()                                                                                                                \
        :data_(nullptr){ current_field_id_ = 0;};                                                                         \
    ~NAME() {clear();};                                                                                                   \
    static constexpr const char* name = #NAME;                                                                            \
private:                                                                                                                  \
    protostruct::ProtoStructEntry<protostruct::field_types::required, uint8_t> current_field_id_;                         \
    void* data_;                                                                                                          \
public:                                                                                                                   \
    typedef NAME class_type;                                                                                              \
    template<size_t id, bool dummy = true>                                                                                \
    struct proto_entry                                                                                                    \
    {                                                                                                                     \
        typedef void value_type;                                                                                          \
        typedef void proto_type;                                                                                          \
        static constexpr const char*  name = "";                                                                          \
        static size_t get(const class_type* ptr) {return 0;};                                                             \
        static constexpr bool is_end = true;                                                                              \
        static bool hasField(const class_type* ptr)  {return false;};                                                     \
    };                                                                                                                    \
	NAME(const NAME& other)                                                                                               \
		:NAME()                                                                                                           \
	{                                                                                                                     \
		this->copy(&other);                                                                                               \
	}                                                                                                                     \
                                                                                                                          \
	NAME(NAME&& other)                                                                                                    \
		:current_field_id_(other.current_field_id_), data_(other.data_)                                                   \
	{                                                                                                                     \
		other.current_field_id_ = 0;                                                                                      \
		other.data_ = nullptr;                                                                                            \
	}                                                                                                                     \
                                                                                                                          \
	NAME& operator=(const NAME& other)                                                                                    \
	{                                                                                                                     \
		if (this == &other)                                                                                               \
		{                                                                                                                 \
			return *this;                                                                                                 \
		}                                                                                                                 \
		else                                                                                                              \
		{                                                                                                                 \
            this->copy(&other);                                                                                           \
		}                                                                                                                 \
		return *this;                                                                                                     \
	}                                                                                                                     \
                                                                                                                          \
	NAME& operator=(NAME&& other)                                                                                         \
	{                                                                                                                     \
		if (this == &other)                                                                                               \
		{                                                                                                                 \
			return *this;                                                                                                 \
		}                                                                                                                 \
		else                                                                                                              \
		{                                                                                                                 \
            clear();                                                                                                      \
			current_field_id_ = other.current_field_id_;                                                                  \
			data_ = other.data_;                                                                                          \
			other.current_field_id_ = 0;                                                                                  \
			other.data_ = nullptr;                                                                                        \
        }                                                                                                                 \
		return *this;                                                                                                     \
	}                                                                                                                     \
private:                                                                                                                  \
    template<size_t id, bool dummy = true>                                                                                \
    struct __proto_helper                                                                                                 \
    {                                                                                                                     \
        static size_t encode(const class_type* ptr, void* buffer)  {return 0;};                                           \
        static size_t decode(class_type* ptr, const void* buffer, size_t length) {return 0;};                             \
		static void copy(class_type* ptr, const class_type* other_ptr) {return;};                                         \
        static size_t encoded_size(const class_type* ptr) {return 0;};                                                    \
        static void create(class_type* ptr) {return;};                                                                    \
        static void destroy(class_type* ptr) {return;};                                                                   \
        static constexpr size_t count()                                                                                   \
        {                                                                                                                 \
            return 0;                                                                                                     \
        }                                                                                                                 \
    };                                       

#define __PROTOUNION_ENTRY(FIELD_TYPE, TYPE, FIELD_NAME, ID, ...)                                                         \
public:                                                                                                                   \
    bool has_##FIELD_NAME() const { return ID == current_field_id_;};                                                     \
                                                                                                                          \
    template<bool dummy>                                                                                                  \
    struct proto_entry<ID, dummy>                                                                                         \
    {                                                                                                                     \
        typedef TYPE value_type;                                                                                          \
        typedef protostruct::ProtoStructEntry<protostruct::field_types::FIELD_TYPE, TYPE, ##__VA_ARGS__> proto_type;      \
        static constexpr bool is_end = false;                                                                             \
        static constexpr const char* name = #FIELD_NAME;                                                                  \
        static bool hasField(const class_type* ptr)  {return ptr->has_##FIELD_NAME();};                                   \
        static const proto_type& get(const class_type* ptr)  {return ptr->FIELD_NAME();};                                 \
    };                                                                                                                    \
private:                                                                                                                  \
    template<bool dummy>                                                                                                  \
    struct __proto_helper<ID, dummy>                                                                                      \
    {                                                                                                                     \
        static void create(class_type* ptr)                                                                               \
        {                                                                                                                 \
            ptr->data_ = new proto_entry<ID>::proto_type();                                                               \
            ptr->current_field_id_ = ID;                                                                                  \
        };                                                                                                                \
                                                                                                                          \
        static void destroy(class_type* ptr)                                                                              \
        {                                                                                                                 \
            if (ptr->current_field_id_ != ID)                                                                             \
            {                                                                                                             \
                return __proto_helper<ID+1>::destroy(ptr);                                                                \
            }                                                                                                             \
            delete static_cast<proto_entry<ID>*>(ptr->data_);                                                             \
            ptr->current_field_id_ = 0;                                                                                   \
        };                                                                                                                \
                                                                                                                          \
        static void copy(class_type* ptr, const class_type* other_ptr)                                                    \
        {                                                                                                                 \
			if (other_ptr->current_field_id_ != ID)                                                                       \
			{                                                                                                             \
			      return __proto_helper<ID + 1>::copy(ptr, other_ptr);                                                    \
			}                                                                                                             \
			create(ptr);                                                                                                  \
            ptr->FIELD_NAME() = other_ptr->FIELD_NAME();                                                                  \
        }                                                                                                                 \
                                                                                                                          \
        static size_t encode(const class_type* ptr, void* buffer)                                                         \
        {                                                                                                                 \
            if (ptr->current_field_id_ != ID)                                                                             \
            {                                                                                                             \
                return __proto_helper<ID+1>::encode(ptr, buffer);                                                         \
            }                                                                                                             \
            return ptr->FIELD_NAME().encode(buffer);                                                                      \
        };                                                                                                                \
                                                                                                                          \
        static size_t decode(class_type* ptr, const void* buffer, size_t length)                                          \
        {                                                                                                                 \
            if (ptr->current_field_id_ != ID)                                                                             \
            {                                                                                                             \
                return __proto_helper<ID+1>::decode(ptr, buffer, length);                                                 \
            }                                                                                                             \
			ptr->clear();                                                                                                 \
            create(ptr);                                                                                                  \
            return ptr->FIELD_NAME().decode(buffer, length);                                                              \
        };                                                                                                                \
                                                                                                                          \
        static size_t encoded_size(const class_type* ptr)                                                                 \
        {                                                                                                                 \
            if (ptr->current_field_id_ != ID)                                                                             \
            {                                                                                                             \
                return  __proto_helper<ID+1>::encoded_size(ptr);                                                          \
            }                                                                                                             \
            return ptr->FIELD_NAME().encodedSize();                                                                      \
        };                                                                                                                \
                                                                                                                          \
        static constexpr size_t count()                                                                                   \
        {   static_assert(ID == 1 || !proto_entry<ID-1>::is_end,                                                          \
            "Not all protounion entries were defined");                                                                   \
            return 1 + __proto_helper<ID+1>::count();                                                                     \
        }                                                                                                                 \
    };                                                                                                                    \
public:                                                                                                                   \
    proto_entry<ID>::proto_type& FIELD_NAME()                                                                             \
    {                                                                                                                     \
        if (current_field_id_ != ID)                                                                                      \
        {                                                                                                                 \
            clear();                                                                                                      \
            __proto_helper<ID>::create(this);                                                                             \
        }                                                                                                                 \
        return *static_cast<proto_entry<ID>::proto_type*>(data_);                                                         \
    };                                                                                                                    \
                                                                                                                          \
    const proto_entry<ID>::proto_type& FIELD_NAME() const                                                                 \
    {                                                                                                                     \
        return *static_cast<const proto_entry<ID>::proto_type*>(data_);                                                   \
    };                                                                                                                    \
                                                                                                                          \
                                                                                                                          \


#define __PROTOUNION_END                                                                                                  \
    template<size_t ID, bool dummy> friend struct __proto_helper;                                                         \
public:                                                                                                                   \
    size_t encode(void* buffer) const                                                                                     \
    {                                                                                                                     \
        uint8_t* buf = static_cast<uint8_t*>(buffer);                                                                     \
        size_t bytes_written = current_field_id_.encode(buffer);                                                          \
        buf += bytes_written;                                                                                             \
        return bytes_written + __proto_helper<1>::encode(this, buf);                                                      \
    };                                                                                                                    \
    size_t decode(const void* buffer, size_t length)                                                                      \
    {                                                                                                                     \
        const uint8_t* buf = static_cast<const uint8_t*>(buffer);                                                         \
        size_t bytes_read = current_field_id_.decode(buffer, length);                                                     \
        if (bytes_read == 0)                                                                                              \
        {                                                                                                                 \
            return 0;                                                                                                     \
        }                                                                                                                 \
        if (current_field_id_ == 0)                                                                                       \
        {                                                                                                                 \
            return bytes_read;                                                                                            \
        }                                                                                                                 \
        buf += bytes_read;                                                                                                \
        size_t opt_bytes_read = __proto_helper<1>::decode(this, buf, length - 1);                                         \
        if (opt_bytes_read == 0)                                                                                          \
        {                                                                                                                 \
            return 0;                                                                                                     \
        }                                                                                                                 \
        return bytes_read + opt_bytes_read;                                                                               \
    };                                                                                                                    \
    size_t encodedSize() const {return current_field_id_.encodedSize() +__proto_helper<1>::encoded_size(this);};          \
    static constexpr  size_t fieldCount(){ return __proto_helper<1>::count();};                                           \
    void clear() {__proto_helper<1>::destroy(this);};                                                                     \
    void copy(const class_type* other)                                                                                    \
    {                                                                                                                     \
		if (this == other)                                                                                                \
		{                                                                                                                 \
			return;                                                                                                       \
		}                                                                                                                 \
		clear();                                                                                                          \
		__proto_helper<1>::copy(this, other);                                                                             \
	}                                                                                                                     \
                                                                                                                          \
};

#endif