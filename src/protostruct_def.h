#ifndef PROTOSTRUCT_DEF_H
#define PROTOSTRUCT_DEF_H

#define __PROTOSTRUCT_BEGIN(NAME)                                                                                         \
class NAME                                                                                                                \
{                                                                                                                         \
public:                                                                                                                   \
    typedef NAME class_type;                                                                                              \
    static constexpr const char* name = #NAME;                                                                            \
    template<size_t id, bool dummy = true>                                                                                \
    struct proto_entry                                                                                                    \
    {                                                                                                                     \
        typedef void value_type;                                                                                          \
        typedef void proto_type;                                                                                          \
        static constexpr const char* name = "";                                                                           \
        static size_t get(const class_type* ptr) {return 0;};                                                             \
        static constexpr bool is_end = true;                                                                              \
    };                                                                                                                    \
private:                                                                                                                  \
    template<size_t id, bool dummy = true>                                                                                \
    struct __proto_helper                                                                                                 \
    {                                                                                                                     \
        static size_t encode(const class_type* ptr, void* buffer)  {return 0;};                                           \
        static size_t decode(class_type* ptr, const void* buffer, size_t length) {return 0;};                             \
        static size_t encoded_size(const class_type* ptr) {return 0;};                                                    \
        static constexpr size_t count()                                                                                   \
        {                                                                                                                 \
            return 0;                                                                                                     \
        }                                                                                                                 \
    };                                       

#define __PROTOSTRUCT_ENTRY(FIELD_TYPE, TYPE, FIELD_NAME, ID, ...)                                                        \
private:                                                                                                                  \
    protostruct::ProtoStructEntry<protostruct::field_types::FIELD_TYPE, TYPE , ##__VA_ARGS__ > FIELD_NAME##_;             \
public:                                                                                                                   \
    protostruct::ProtoStructEntry<protostruct::field_types::FIELD_TYPE, TYPE , ##__VA_ARGS__ >& FIELD_NAME()              \
    {                                                                                                                     \
        return FIELD_NAME##_;                                                                                             \
    };                                                                                                                    \
                                                                                                                          \
    const protostruct::ProtoStructEntry<protostruct::field_types::FIELD_TYPE, TYPE , ##__VA_ARGS__>& FIELD_NAME() const   \
    {                                                                                                                     \
        return FIELD_NAME##_;                                                                                             \
    };                                                                                                                    \
                                                                                                                          \
    template<bool dummy>                                                                                                  \
    struct proto_entry<ID, dummy>                                                                                         \
    {                                                                                                                     \
        typedef TYPE value_type;                                                                                          \
        typedef protostruct::ProtoStructEntry<protostruct::field_types::FIELD_TYPE, TYPE, ##__VA_ARGS__> proto_type;      \
        static constexpr bool is_end = false;                                                                             \
        static constexpr const char* name = #FIELD_NAME;                                                                  \
        static const proto_type& get(const class_type* ptr)  {return ptr->FIELD_NAME();};                                 \
    };                                                                                                                    \
private:                                                                                                                  \
    template<bool dummy>                                                                                                  \
    struct __proto_helper<ID, dummy>                                                                                      \
    {                                                                                                                     \
        static size_t encode(const class_type* ptr, void* buffer)                                                         \
        {                                                                                                                 \
            size_t bytes_written = ptr->FIELD_NAME().encode(buffer);                                                      \
            uint8_t* buf = static_cast<uint8_t*>(buffer);                                                                 \
            return bytes_written +__proto_helper<ID+1>::encode(ptr, buf + bytes_written);                                 \
        };                                                                                                                \
                                                                                                                          \
        static size_t decode(class_type* ptr, const void* buffer, size_t length)                                          \
        {                                                                                                                 \
            size_t bytes_read = ptr->FIELD_NAME().decode(buffer, length);                                                 \
            if (bytes_read == 0)                                                                                          \
            {                                                                                                             \
                return 0;                                                                                                 \
            }                                                                                                             \
            const uint8_t* buf = static_cast<const uint8_t*>(buffer);                                                     \
            buf += bytes_read;                                                                                            \
            length -= bytes_read;                                                                                         \
            return bytes_read + __proto_helper<ID+1>::decode(ptr, buf, length);                                           \
        };                                                                                                                \
                                                                                                                          \
        static size_t encoded_size(const class_type* ptr)                                                                 \
        {                                                                                                                 \
            return ptr->FIELD_NAME().encodedSize() + __proto_helper<ID+1>::encoded_size(ptr);                            \
        };                                                                                                                \
                                                                                                                          \
        static constexpr size_t count()                                                                                   \
        {   static_assert(ID == 1 || !proto_entry<ID-1>::is_end,                                                          \
            "Not all protostruct entries were defined");                                                                  \
            return 1 + __proto_helper<ID+1>::count();                                                                     \
        }                                                                                                                 \
    };

#define __PROTOSTRUCT_END                                                                                                 \
public:                                                                                                                   \
    size_t encode(void* buffer) const {return __proto_helper<1>::encode(this, buffer);};                                  \
    size_t decode(const void* buffer, size_t length) {return __proto_helper<1>::decode(this, buffer, length);};           \
    size_t encodedSize() const {return __proto_helper<1>::encoded_size(this);};                                          \
    static constexpr  size_t fieldCount(){ return __proto_helper<1>::count();};                                           \
};

#endif