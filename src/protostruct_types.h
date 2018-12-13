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
        
    template<class S = repeated_size_type>  struct repeated_opt{};       
    template<size_t N> struct repeated_fixed{};   
    typedef repeated_opt<repeated_size_type> repeated;
}


template <class FT, class T, class Enc = T> class ProtoStructEntry;

namespace aux
{
template <class T, class X>
class ProtoStructEntryBase
{
    protected:
        X value_;
    public:
        void set(const T& val) { value_ = static_cast<X>(val);};
        T get() const {return static_cast<T>(value_);};
        size_t encodedSize() const {return value_.encodedSize();};

        ProtoStructEntryBase& operator=(const T& new_val)
        {
            value_ = static_cast<X>(new_val);
            return *this;
        }

        ProtoStructEntryBase& operator=(T&& new_val)
        {
            value_ = static_cast<X>(new_val);
            return *this;
        }

        operator T() const { return static_cast<T>(value_); };       
};


template <class T>
class ProtoStructEntryBase<T, T>
{
    protected:
        T value_;
    public:
        void set(const T& val) { value_ = val;};
        const T& get() const {return value_;};
        T& ref() {return value_;};

        size_t encodedSize() const {return value_.encodedSize();};

        ProtoStructEntryBase& operator=(const T& new_val)
        {
            value_ = new_val;
            return *this;
        }

        ProtoStructEntryBase& operator=(T&& new_val)
        {
            value_ = new_val;
            return *this;
        }

        operator T() const { return value_; };
};

}

template <class T, class X>
class ProtoStructEntry<field_types::required, T, X>: public aux::ProtoStructEntryBase<T,X>
{
    public:
        size_t encode(void* buffer) const
        {
            return this->value_.encode(buffer);
        }
        
        size_t decode(const void* buffer, size_t length)
        {
            return this->value_.decode(buffer, length);
        }

        ProtoStructEntry& operator=(const T& new_val)
        {
            aux::ProtoStructEntryBase<T,X>::operator=(new_val);
            return *this;
        }

        ProtoStructEntry& operator=(T&& new_val)
        {
            aux::ProtoStructEntryBase<T,X>::operator=(new_val);
            return *this;
        }

};


template <class T>
class ProtoStructEntry<field_types::required, T, bool>
{
    protected:
        bool value_;
    public:
        void set(const T& val) { value_ = static_cast<bool>(val);};
        const T get() const {return static_cast<T>(value_);};
        size_t encodedSize() const {return sizeof(uint8_t);};

        size_t encode(void* buffer) const
        {
            *reinterpret_cast<bool*>(buffer) = value_;
            return 1;
        }
        
        size_t decode(const void* buffer, size_t length)
        {
            if (length == 0)
            {
                return 0;
            }
            value_ = *reinterpret_cast<const bool*>(buffer);
            return 1;
        }

        ProtoStructEntry& operator=(const T& new_val)
        {
            value_ = static_cast<bool>(new_val);
            return *this;
        }

        operator T() const { return static_cast<T>(value_); };
};



template <class T>
class ProtoStructEntry<field_types::required, T, uint8_t>
{
    protected:
        uint8_t value_;
    public:
        void set(const T& val) { value_ = static_cast<uint8_t>(val);};
        const T get() const {return static_cast<T>(value_);};
        size_t encodedSize() const {return sizeof(uint8_t);};

        size_t encode(void* buffer) const
        {
            *reinterpret_cast<uint8_t*>(buffer) = value_;
            return 1;
        }
        
        size_t decode(const void* buffer, size_t length)
        {
            if (length == 0)
            {
                return 0;
            }
            value_ = *reinterpret_cast<const uint8_t*>(buffer);
            return 1;
        }

        ProtoStructEntry& operator=(const T& new_val)
        {
            value_ = static_cast<uint8_t>(new_val);
            return *this;
        }

        operator T() const { return static_cast<T>(value_); };
};


template <class T>
class ProtoStructEntry<field_types::required, T, int8_t>
{
    protected:
        int8_t value_;
    public:
        void set(const T& val) { value_ = static_cast<int8_t>(val);};
        const T get() const {return static_cast<T>(value_);};
        size_t encodedSize() const {return sizeof(int8_t);};

        size_t encode(void* buffer) const
        {
            uint8_t enc = value_ > 0 ? 1 : 0;
            uint8_t magn = value_ > 0 ? value_ : -value_;
            enc += (magn << 1u);
            *reinterpret_cast<uint8_t*>(buffer) = enc;
            return 1;
        }
        
        size_t decode(const void* buffer, size_t length)
        {
            if (length == 0)
            {
                return 0;
            }
            uint8_t val = *reinterpret_cast<const uint8_t*>(buffer);
            value_ = (val >> 1u);
            value_ = (val & 1u) ? value_ : -value_;
            return 1;
        }

        ProtoStructEntry& operator=(const T& new_val)
        {
            value_ = static_cast<int8_t>(new_val);
            return *this;
        }

        operator T() const { return static_cast<T>(value_); };
};


template <class T>
class ProtoStructEntry<field_types::required, T, uint32_t>
{
    protected:
        uint32_t value_;
    public:
        void set(const T& val) { value_ = static_cast<uint32_t>(val);};
        T get() const {return value_;};
        size_t encodedSize() const {return sizeof(uint32_t);};

        size_t encode(void* buffer) const
        {
            uint8_t* buf = reinterpret_cast<uint8_t*>(buffer);
            buf[0] = value_ & 0b11111111u;
            buf[1] = (value_ >> 8) & 0b11111111u;
            buf[2] = (value_ >> 16) & 0b11111111u;
            buf[3] = (value_ >> 24) & 0b11111111u;

            return 4;
        }
        
        size_t decode(const void* buffer, size_t length)
        {
            if (length < 4)
            {
                return 0;
            }
            const uint8_t* buf = reinterpret_cast<const uint8_t*>(buffer);
            value_ = 0;
            value_ += static_cast<uint32_t>(buf[0]);
            value_ += static_cast<uint32_t>(buf[1]) << 8;
            value_ += static_cast<uint32_t>(buf[2]) << 16;
            value_ += static_cast<uint32_t>(buf[3]) << 24;
            return 4;
        }

        ProtoStructEntry& operator=(const T& new_val)
        {
            value_ = static_cast<uint32_t>(new_val);
			return *this;
        }

        operator T() const { return static_cast<T>(value_); };
};


template <class T>
class ProtoStructEntry<field_types::required, T, int32_t>
{
    protected:
        int32_t value_;
    public:
        void set(const T& val) { value_ = static_cast<T>(val);};
        T get() const {return value_;};
        size_t encodedSize() const {return sizeof(int32_t);};

        size_t encode(void* buffer) const
        {
            uint8_t* buf = reinterpret_cast<uint8_t*>(buffer);
            uint32_t val = value_ >0 ? 1 : 0;
            uint32_t magn = value_ > 0 ? value_ : -value_;
            val += (magn << 1u);

            buf[0] = val & 0b11111111u;
            buf[1] = (val >> 8) & 0b11111111u;
            buf[2] = (val >> 16) & 0b11111111u;
            buf[3] = (val >> 24) & 0b11111111u;

            return 4;
        }
        
        size_t decode(const void* buffer, size_t length)
        {
            if (length < 4)
            {
                return 0;
            }
            const uint8_t* buf = reinterpret_cast<const uint8_t*>(buffer);
            uint32_t val;
            val = 0;
            val += static_cast<uint32_t>(buf[0]);
            val += static_cast<uint32_t>(buf[1]) << 8;
            val += static_cast<uint32_t>(buf[2]) << 16;
            val += static_cast<uint32_t>(buf[3]) << 24;
            value_ = (val >> 1u);
            value_ = (val & 1u) ? value_ : -value_;
            return 4;
        }
        
        ProtoStructEntry& operator=(const T& new_val)
        {
            value_ = static_cast<int32_t>(new_val);
			return *this;
        }

        operator T() const { return static_cast<T>(value_); };
};


template <class T>
class ProtoStructEntry<field_types::required, T, uint16_t>
{
    protected:
        uint16_t value_;
    public:
        void set(const T& val) { value_ = static_cast<uint16_t>(val);};
        T get() const {return static_cast<T>(value_);};
        size_t encodedSize() const {return sizeof(uint16_t);};

        size_t encode(void* buffer) const
        {
            uint8_t* buf = reinterpret_cast<uint8_t*>(buffer);
            buf[0] = value_ & 0b11111111u;
            buf[1] = (value_ >> 8) & 0b11111111u;

            return 2;
        }
        
        size_t decode(const void* buffer, size_t length)
        {
            if (length < 2)
            {
                return 0;
            }

            const uint8_t* buf = reinterpret_cast<const uint8_t*>(buffer);
            value_ = 0;
            value_ += static_cast<uint32_t>(buf[0]);
            value_ += static_cast<uint32_t>(buf[1]) << 8;
            return 2;
        }

        ProtoStructEntry& operator=(const T& new_val)
        {
            value_ = static_cast<uint16_t>(new_val);
			return *this;
        }

        operator T() const { return static_cast<T>(value_); };
};


template <class T>
class ProtoStructEntry<field_types::required, T, int16_t>
{
    protected:
        int16_t value_;
    public:
        void set(const T& val) { value_ = static_cast<int16_t>(val);};
        T get() const {return static_cast<T>(value_);};
        size_t encodedSize() const {return sizeof(int16_t);};

       size_t encode(void* buffer) const
        {
            uint8_t* buf = reinterpret_cast<uint8_t*>(buffer);
            uint16_t val = value_ >0 ? 1 : 0;
            uint16_t magn = value_ > 0 ? value_ : -value_;
            val += (magn << 1u);

            buf[0] = val & 0b11111111u;
            buf[1] = (val >> 8) & 0b11111111u;

            return 2;
        }
        
        size_t decode(const void* buffer, size_t length)
        {
            if (length < 2)
            {
                return 0;
            }
            const uint8_t* buf = reinterpret_cast<const uint8_t*>(buffer);
            uint16_t val;
            val = 0;
            val += static_cast<uint16_t>(buf[0]);
            val += static_cast<uint16_t>(buf[1]) << 8;
            value_ = (val >> 1u);
            value_ = (val & 1u) ? value_ : -value_;
            return 2;
        }
        
        ProtoStructEntry& operator=(const T& new_val)
        {
            value_ = static_cast<int16_t>(new_val);
			return *this;
        }

        operator T() const { return static_cast<T>(value_); };
};


template <class T>
class ProtoStructEntry<field_types::required, T, float>
{
    protected:
        float value_;
    public:
        void set(const T& val) { value_ = static_cast<float>(val);};
        T get() const {return static_cast<T>(value_);};
        size_t encodedSize() const {return sizeof(float);};

        size_t encode(void* buffer) const
        {
            uint8_t* buf = reinterpret_cast<uint8_t*>(buffer);
            const uint32_t value = reinterpret_cast<const uint32_t&>(value_);
            buf[0] = value & 0b11111111u;
            buf[1] = (value >> 8) & 0b11111111u;
            buf[2] = (value >> 16) & 0b11111111u;
            buf[3] = (value >> 24) & 0b11111111u;

            return 4;
        }
        
        size_t decode(const void* buffer, size_t length)
        {
            if (length < 4)
            {
                return 0;
            }
            const uint8_t* buf = reinterpret_cast<const uint8_t*>(buffer);
            uint32_t value = 0;
            value += static_cast<uint32_t>(buf[0]);
            value += static_cast<uint32_t>(buf[1]) << 8;
            value += static_cast<uint32_t>(buf[2]) << 16;
            value += static_cast<uint32_t>(buf[3]) << 24;
            value_ = reinterpret_cast<float&>(value);
            return 4;
        }

        ProtoStructEntry& operator=(const T& new_val)
        {
            value_ = static_cast<float>(new_val);
			return *this;
        }

        operator T() const { return static_cast<T>(value_); };
};


template <class T, class X>
class ProtoStructEntry<field_types::optional, T, X> : public  ProtoStructEntry<field_types::required, T, X>
{
    private:
        bool is_set_;
    public:
        ProtoStructEntry()
            :is_set_(false){};

        void set(const T& val) {ProtoStructEntry<field_types::required, T, X>::set(val); is_set_ = true;};
        void unset() {is_set_ = false;};
        bool isSet() const {return is_set_;};

        size_t encodedSize() const {return 1 + is_set_ ? ProtoStructEntry<field_types::required, T, X>::encodedSize() : 0;};

        size_t encode(void* buffer) const
        {
            size_t bytes_written = 1;
            uint8_t* buf = reinterpret_cast<uint8_t*>(buffer);
            buf[0] = is_set_;
            buf++;
            if (is_set_)
            {
                bytes_written += ProtoStructEntry<field_types::required, T, X>::encode(buf);
            }
            return bytes_written;
        }
        
        size_t decode(const void* buffer, size_t length)
        {
            if (length < 1)
            {
                return 0;
            }
            size_t bytes_read = 1;
            const uint8_t* buf = reinterpret_cast<const uint8_t*>(buffer);
            is_set_ = buf[0];
            buf++;
            length--;

            if (is_set_)
            {
                bytes_read += ProtoStructEntry<field_types::required, T, X>::decode(buf, length);
                if (bytes_read == 1)
                {
                    return 0;
                }
            }
            return bytes_read;
        }

        ProtoStructEntry& operator=(const T& new_val)
        {
            this->value_ = static_cast<X>(new_val);
            is_set_ = true;
			return *this;
        }
};


template <class T>
class ProtoStructEntry<field_types::optional, T, T> : public  ProtoStructEntry<field_types::required, T, T>
{
    private:
        bool is_set_;
    public:
        ProtoStructEntry()
            :is_set_(false){};

        void set(const T& val) {ProtoStructEntry<field_types::required, T, T>::set(val); is_set_ = true;};
        void unset() {is_set_ = false;};
        void markSet() {is_set_ = true;};
        bool isSet() const {return is_set_;};

        size_t encodedSize() const {return 1 + is_set_*ProtoStructEntry<field_types::required, T>::encodedSize();};

        size_t encode(void* buffer) const
        {
            size_t bytes_written = 1;
            uint8_t* buf = reinterpret_cast<uint8_t*>(buffer);
            buf[0] = is_set_;
            buf++;
            if (is_set_)
            {
                bytes_written += ProtoStructEntry<field_types::required, T>::encode(buf);
            }
            return bytes_written;
        }
        
        size_t decode(const void* buffer, size_t length)
        {
            if (length < 1)
            {
                return 0;
            }
            size_t bytes_read = 1;

            const uint8_t* buf = reinterpret_cast<const uint8_t*>(buffer);
            is_set_ = buf[0];
            buf++;
            length--;
            if (is_set_)
            {
                bytes_read += ProtoStructEntry<field_types::required, T>::decode(buf, length);
                if (bytes_read == 1)
                {
                    return 0;
                }
            }
            return bytes_read;
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
};


template <class T, class X,  size_t N>
class ProtoStructEntry<field_types::repeated_fixed<N>, T, X>
{
    private:
        ProtoStructEntry<field_types::required, T, X> data_[N];
    public:
        ProtoStructEntry(){};

        const ProtoStructEntry<field_types::required, T>* data() const {return data_;};
        ProtoStructEntry<field_types::required, T>* data() {return data_;};

        constexpr size_t size() const {return N;}
        size_t encodedSize() const 
        {
            size_t sz = sizeof(repeated_size_type);
            for (size_t i = 0; i < N; i++)
            {
                sz += data_[i].encodedSize();
            }
            return sz;
        }


        size_t encode(void* buffer) const
        {
            size_t bytes_written = 0;
            uint8_t* buf = static_cast<uint8_t*>(buffer);
            
            for (size_t i = 0; i< N; i++)
            {
                size_t bytes_written_i = data_[i].encode(buf);
                buf += bytes_written_i;
                bytes_written += bytes_written_i;
            }
            return bytes_written;
        }

        
        size_t decode(const void* buffer, size_t length)
        {
            const uint8_t* buf = static_cast<const uint8_t*>(buffer);
            size_t bytes_read = 0;

            for (size_t i = 0; i< N; i++)
            {
                size_t bytes_read_i = data_[i].decode(buf, length);
                if (bytes_read_i == 0)
                {
                    return 0;
                }
                length -= bytes_read_i;
                buf += bytes_read_i;
                bytes_read += bytes_read_i;
            }
            return bytes_read;            
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
            if (this == &other)
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
            if (this == &other)
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


template <class T, class X, class S>
class ProtoStructEntry<field_types::repeated_opt<S>, T, X>
{
    private:
        ProtoStructEntry<field_types::required, T, X>* data_;
        ProtoStructEntry<field_types::required, S, S> size_;
    public:
        ProtoStructEntry()
            :data_(nullptr) {size_ = 0;};

        void set_buffer(void* buffer) {data_ = reinterpret_cast<ProtoStructEntry<field_types::required, T>*>(buffer);};
        void set_size(S sz) {size_ = sz;};
        const ProtoStructEntry<field_types::required, T, X>* data() const {return data_;};
        ProtoStructEntry<field_types::required, T, X>* data() {return data_;};

        size_t size() const {return size_;}
        size_t encodedSize() const 
        {
            size_t sz = size_.encodedSize();
            for (size_t i = 0; i < size_; i++)
            {
                sz += data_[i].encodedSize();
            }
            return sz;
        }


        size_t encode(void* buffer) const
        {
            uint8_t* buf = static_cast<uint8_t*>(buffer);
            size_t bytes_written =  size_.encode(buf);
            buf += bytes_written;

            for (size_t i = 0; i< size_.get(); i++)
            {
                size_t bytes_written_i = data_[i].encode(buf);
                buf += bytes_written_i;
                bytes_written += bytes_written_i;
            }
            return bytes_written;
        }

        
        size_t decode(const void* buffer, size_t length)
        {
            const uint8_t* buf = static_cast<const uint8_t*>(buffer);
            size_t bytes_read = size_.decode(buf, length);

            if (bytes_read == 0)
            {
                return 0;
            }
            length -= bytes_read;
            buf += bytes_read;

            for (size_t i = 0; i< size_; i++)
            {
                size_t bytes_read_i = data_[i].decode(buf, length);
                if (bytes_read_i == 0)
                {
                    return 0;
                }
                bytes_read += bytes_read_i;
                buf += bytes_read_i;
                length -= bytes_read_i;
            }
            return bytes_read;            
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
            if (this == &other)
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
            if (this == &other)
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
