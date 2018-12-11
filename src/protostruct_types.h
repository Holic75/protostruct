#ifndef PROTOSTRUCT_TYPES_H
#define PROTOSTRUCT_TYPES_H

#ifndef ARDUINO
#include <cstdint>
#include <stddef.h>
#endif

namespace protostruct 
{


typedef uint16_t repeated_size_type;

namespace field_types
{
    struct required{};    
    struct optional{};       
    struct repeated{};       
    template<size_t N> struct repeated_fixed{};   
}



template <class FT, class T> class ProtoStructEntry;



template <class T>
class ProtoStructEntry<field_types::required, T>
{
    protected:
        T value_;
    public:
        void set(const T& val) { value_ = val;};
        const T& get() const {return value_;};
        T& ref() {return value_;};
        size_t encoded_size() const {return value_.encoded_size();};

        void* encode(void* buffer) const
        {
            return value_.encode(buffer);
        }
        
        const void* decode(const void* buffer)
        {
            return value_.decode(buffer);
        }

        ProtoStructEntry& operator=(const T& new_val)
        {
            value_ = new_val;
        }

        ProtoStructEntry& operator=(T&& new_val)
        {
            value_ = new_val;
        }

        operator T() const { return value_; };
};


template <>
class ProtoStructEntry<field_types::required, uint8_t>
{
    protected:
        uint8_t value_;
    public:
        void set(uint8_t val) { value_ = val;};
        const uint8_t& get() const {return value_;};
        uint8_t& ref() {return value_;};
        size_t encoded_size() const {return sizeof(uint8_t);};

        void* encode(void* buffer) const
        {
            *reinterpret_cast<uint8_t*>(buffer) = value_;
            return reinterpret_cast<uint8_t*>(buffer) + 1;
        }
        
        const void* decode(const void* buffer)
        {
            value_ = *reinterpret_cast<const uint8_t*>(buffer);
            return reinterpret_cast<const uint8_t*>(buffer) + 1;
        }

        ProtoStructEntry& operator=(const uint8_t new_val)
        {
            value_ = new_val;
        }

        operator uint8_t() const { return value_; };
};


template <>
class ProtoStructEntry<field_types::required, int8_t>
{
    protected:
        int8_t value_;
    public:
        void set(int8_t val) { value_ = val;};
        const int8_t& get() const {return value_;};
        int8_t& ref() {return value_;};
        size_t encoded_size() const {return sizeof(int8_t);};

        void* encode(void* buffer) const
        {
            uint8_t enc = value_ > 0 ? 1 : 0;
            uint8_t magn = value_ > 0 ? value_ : -value_;
            enc += (magn << 1u);
            *reinterpret_cast<uint8_t*>(buffer) = enc;
            return reinterpret_cast<uint8_t*>(buffer) + 1;
        }
        
        const void* decode(const void* buffer)
        {
            uint8_t val = *reinterpret_cast<const uint8_t*>(buffer);
            value_ = (val >> 1u);
            value_ = (val & 1u) ? value_ : -value_;
            return reinterpret_cast<const uint8_t*>(buffer) + 1;
        }

        ProtoStructEntry& operator=(const int8_t new_val)
        {
            value_ = new_val;
        }

        operator int8_t() const { return value_; };
};


template <>
class ProtoStructEntry<field_types::required, uint32_t>
{
    protected:
        uint32_t value_;
    public:
        void set(uint32_t val) { value_ = val;};
        const uint32_t& get() const {return value_;};
        uint32_t& ref() {return value_;};
        size_t encoded_size() const {return sizeof(uint32_t);};

        void* encode(void* buffer) const
        {
            uint8_t* buf = reinterpret_cast<uint8_t*>(buffer);
            buf[0] = value_ & 0b11111111u;
            buf[1] = (value_ >> 8) & 0b11111111u;
            buf[2] = (value_ >> 16) & 0b11111111u;
            buf[3] = (value_ >> 24) & 0b11111111u;

            return buf + 4;
        }
        
        const void* decode(const void* buffer)
        {
            const uint8_t* buf = reinterpret_cast<const uint8_t*>(buffer);
            value_ = 0;
            value_ += static_cast<uint32_t>(buf[0]);
            value_ += static_cast<uint32_t>(buf[1]) << 8;
            value_ += static_cast<uint32_t>(buf[2]) << 16;
            value_ += static_cast<uint32_t>(buf[3]) << 24;
            return buf + 4;
        }

        ProtoStructEntry& operator=(const uint32_t new_val)
        {
            value_ = new_val;
			return *this;
        }

        operator uint32_t() const { return value_; };
};


template <>
class ProtoStructEntry<field_types::required, int32_t>
{
    protected:
        int32_t value_;
    public:
        void set(int32_t val) { value_ = val;};
        const int32_t& get() const {return value_;};
        int32_t& ref() {return value_;};
        size_t encoded_size() const {return sizeof(int32_t);};

        void* encode(void* buffer) const
        {
            uint8_t* buf = reinterpret_cast<uint8_t*>(buffer);
            uint32_t val = value_ >0 ? 1 : 0;
            uint32_t magn = value_ > 0 ? value_ : -value_;
            val += (magn << 1u);

            buf[0] = val & 0b11111111u;
            buf[1] = (val >> 8) & 0b11111111u;
            buf[2] = (val >> 16) & 0b11111111u;
            buf[3] = (val >> 24) & 0b11111111u;

            return buf + 4;
        }
        
        const void* decode(const void* buffer)
        {
            const uint8_t* buf = reinterpret_cast<const uint8_t*>(buffer);
            uint32_t val;
            val = 0;
            val += static_cast<uint32_t>(buf[0]);
            val += static_cast<uint32_t>(buf[1]) << 8;
            val += static_cast<uint32_t>(buf[2]) << 16;
            val += static_cast<uint32_t>(buf[3]) << 24;
            value_ = (val >> 1u);
            value_ = (val & 1u) ? value_ : -value_;
            return buf + 4;
        }
        
        ProtoStructEntry& operator=(const int32_t new_val)
        {
            value_ = new_val;
			return *this;
        }

        operator int32_t() const { return value_; };
};


template <>
class ProtoStructEntry<field_types::required, uint16_t>
{
    protected:
        uint16_t value_;
    public:
        void set(uint16_t val) { value_ = val;};
        const uint16_t& get() const {return value_;};
        uint16_t& ref() {return value_;};
        size_t encoded_size() const {return sizeof(uint16_t);};

        void* encode(void* buffer) const
        {
            uint8_t* buf = reinterpret_cast<uint8_t*>(buffer);
            buf[0] = value_ & 0b11111111u;
            buf[1] = (value_ >> 8) & 0b11111111u;

            return buf + 2;
        }
        
        const void* decode(const void* buffer)
        {
            const uint8_t* buf = reinterpret_cast<const uint8_t*>(buffer);
            value_ = 0;
            value_ += static_cast<uint32_t>(buf[0]);
            value_ += static_cast<uint32_t>(buf[1]) << 8;
            return buf + 2;
        }

        ProtoStructEntry& operator=(const uint16_t new_val)
        {
            value_ = new_val;
			return *this;
        }

        operator uint16_t() const { return value_; };
};


template <>
class ProtoStructEntry<field_types::required, int16_t>
{
    protected:
        int16_t value_;
    public:
        void set(int16_t val) { value_ = val;};
        const int16_t& get() const {return value_;};
        int16_t& ref() {return value_;};
        size_t encoded_size() const {return sizeof(int16_t);};

        void* encode(void* buffer) const
        {
            uint8_t* buf = reinterpret_cast<uint8_t*>(buffer);
            uint16_t val = value_ >0 ? 1 : 0;
            uint16_t magn = value_ > 0 ? value_ : -value_;
            val += (magn << 1u);

            buf[0] = val & 0b11111111u;
            buf[1] = (val >> 8) & 0b11111111u;

            return buf + 2;
        }
        
        const void* decode(const void* buffer)
        {
            const uint8_t* buf = reinterpret_cast<const uint8_t*>(buffer);
            uint16_t val;
            val = 0;
            val += static_cast<uint16_t>(buf[0]);
            val += static_cast<uint16_t>(buf[1]) << 8;
            value_ = (val >> 1u);
            value_ = (val & 1u) ? value_ : -value_;
            return buf + 2;
        }
        
        ProtoStructEntry& operator=(const int16_t new_val)
        {
            value_ = new_val;
			return *this;
        }

        operator int16_t() const { return value_; };
};


template <>
class ProtoStructEntry<field_types::required, float>
{
    protected:
        float value_;
    public:
        void set(float val) { value_ = val;};
        const float& get() const {return value_;};
        float& ref() {return value_;};
        size_t encoded_size() const {return sizeof(float);};

        void* encode(void* buffer) const
        {
            uint8_t* buf = reinterpret_cast<uint8_t*>(buffer);
            const uint32_t value = reinterpret_cast<const uint32_t&>(value_);
            buf[0] = value & 0b11111111u;
            buf[1] = (value >> 8) & 0b11111111u;
            buf[2] = (value >> 16) & 0b11111111u;
            buf[3] = (value >> 24) & 0b11111111u;

            return buf + 4;
        }
        
        const void* decode(const void* buffer)
        {
            const uint8_t* buf = reinterpret_cast<const uint8_t*>(buffer);
            uint32_t value = 0;
            value += static_cast<uint32_t>(buf[0]);
            value += static_cast<uint32_t>(buf[1]) << 8;
            value += static_cast<uint32_t>(buf[2]) << 16;
            value += static_cast<uint32_t>(buf[3]) << 24;
            value_ = reinterpret_cast<float&>(value);
            return buf + 4;
        }

        ProtoStructEntry& operator=(const float new_val)
        {
            value_ = new_val;
			return *this;
        }

        operator float() const { return value_; };
};


template <class T>
class ProtoStructEntry<field_types::optional, T> : public  ProtoStructEntry<field_types::required, T>
{
    private:
        bool is_set_;
    public:
        ProtoStructEntry()
            :is_set_(false){};

        void set(const T& val) {ProtoStructEntry<field_types::required, T>::set(val); is_set_ = true;};
        void unset(uint8_t val) {is_set_ = false;};
        bool isSet() const {return is_set_;};

        size_t encoded_size() const {return 1 + is_set_*ProtoStructEntry<field_types::required, T>::encoded_size();};

        void* encode(void* buffer) const
        {
            uint8_t* buf = reinterpret_cast<uint8_t*>(buffer);
            buf[0] = is_set_;
            buf++;
            if (is_set_)
            {
                buf = reinterpret_cast<uint8_t*>(ProtoStructEntry<field_types::required, T>::encode(buf));
            }
            return buf;
        }
        
        const void* decode(const void* buffer)
        {
            const uint8_t* buf = reinterpret_cast<const uint8_t*>(buffer);
            is_set_ = buf[0];
            buf++;

            if (is_set_)
            {
                buf = reinterpret_cast<const uint8_t*>(ProtoStructEntry<field_types::required, T>::decode(buf));
            }
            return buf;
        }

        ProtoStructEntry& operator=(const T& new_val)
        {
            this->value_ = new_val;
            is_set_ = true;
			return *this;
        }

        ProtoStructEntry& operator=(T&& new_val)
        {
            this->value_ = new_val;
            is_set_ = true;
			return *this;
        }


        operator T() const { return this->value_; };
};


template <class T, size_t N>
class ProtoStructEntry<field_types::repeated_fixed<N>, T>
{
    private:
        ProtoStructEntry<field_types::required, T> data_[N];
    public:
        ProtoStructEntry(){};

        const ProtoStructEntry<field_types::required, T>* data() const {return data_;};
        ProtoStructEntry<field_types::required, T>* data() {return data_;};

        constexpr size_t size() const {return N;}
        size_t encoded_size() const 
        {
            size_t sz = sizeof(repeated_size_type);
            for (size_t i = 0; i < N; i++)
            {
                sz += data_[i].encoded_size();
            }
            return sz;
        }


        void* encode(void* buffer) const
        {
            void* buf = buffer;
            
            for (size_t i = 0; i< N; i++)
            {
                buf = data_[i].encode(buf);
            }
            return buf;
        }

        
        const void* decode(const void* buffer)
        {
            const void* buf = buffer;

            for (size_t i = 0; i< N; i++)
            {
                buf = data_[i].decode(buf);
            }
            return buf;            
        }

        ProtoStructEntry(ProtoStructEntry&& other)
        {
            for (size_t i = 0; i < N; i++)
            {
                data_[i] = other.data[i];
            }
        }

        ProtoStructEntry& operator=(const ProtoStructEntry& other)
        {
            if (this == other)
            {
                return *this;
            }
            else
            {
                for (size_t i = 0; i< N; i++)
                {
                    data_[i].set(other.data_[i].get());
                }
            }
            return *this;
        }


        ProtoStructEntry& operator=(ProtoStructEntry&& other)
        {
            if (this == other)
            {
                return *this;
            }
            else
            {
                for (size_t i = 0; i< N; i++)
                {
                    data_[i].set(other.data_[i].get());
                }
            }

            return *this;
        }

};


template <class T>
class ProtoStructEntry<field_types::repeated, T>
{
    private:
        ProtoStructEntry<field_types::required, T>* data_;
        ProtoStructEntry<field_types::required, repeated_size_type> size_;
    public:
        ProtoStructEntry()
            :data_(nullptr) {size_ = 0;};

        void set_buffer(void* buffer) {data_ = reinterpret_cast<ProtoStructEntry<field_types::required, T>*>(buffer);};
        void set_size(repeated_size_type sz) {size_ = sz;};
        const ProtoStructEntry<field_types::required, T>* data() const {return data_;};
        ProtoStructEntry<field_types::required, T>* data() {return data_;};

        repeated_size_type size() const {return size_;}
        size_t encoded_size() const 
        {
            size_t sz = size_.encoded_size();
            for (size_t i = 0; i < size_; i++)
            {
                sz += data_[i].encoded_size();
            }
            return sz;
        }


        void* encode(void* buffer) const
        {
            void* buf = buffer;

            buf = size_.encode(buf);

            for (size_t i = 0; i< size_.get(); i++)
            {
                buf = (data_[i].encode(buf));
            }
            return buf;
        }

        
        const void* decode(const void* buffer)
        {
            const void* buf = buffer;
            buf = size_.decode(buf);

            for (size_t i = 0; i< size_; i++)
            {
                buf = data_[i].decode(buf);
            }
            return buf;            
        }


        ProtoStructEntry(const ProtoStructEntry& other) = delete;

        ProtoStructEntry(ProtoStructEntry&& other)
            :size_(other.size), data_(other.data)
        {
            other.size_ = 0;
            other.data_ = nullptr;
        }

        ProtoStructEntry& operator=(const ProtoStructEntry& other)
        {
            if (this == other)
            {
                return *this;
            }
            else
            {
                size_ = other.size();
                for (size_t i = 0; i< size_; i++)
                {
                    data_[i].set(other.data_[i].get());
                }
            }
            return *this;
        }


        ProtoStructEntry& operator=(ProtoStructEntry&& other)
        {
            if (this == other)
            {
                return *this;
            }
            else
            {
                size_ = other.size();
                data_ = other.data_;
                other.size_ = 0;
                other.data_ = nullptr;
            }

            return *this;
        }

};




}


#endif
