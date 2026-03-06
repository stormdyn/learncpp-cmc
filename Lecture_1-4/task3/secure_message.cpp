#include <cstddef>
#include <ostream>
#include <stdexcept>
#include <utility>
#include "secure_message.hpp"

namespace context7
{
    void SecureMessage::copy_buffer(const char* src, size_t len) 
    {
        for (size_t i = 0; i < len; ++i) {
            data_[i] = src[i];
        }
        data_[len] = '\0';
    }

    size_t SecureMessage::compute_length(const char* str)  
    {
        if (!str) {
            return 0;
        }
        size_t len = 0;
        while (str[len] != '\0') {
            ++len;
        }
        return len;
    }

    SecureMessage::SecureMessage() : data_(nullptr), length_(0) {}

    SecureMessage::SecureMessage(const char* str)
    {
        if (!str) {
            data_ = nullptr;
            length_ = 0;
            return;
        }
        
        length_ = compute_length(str);
        data_ = new char[length_ + 1]; // Если нет памяти, вылетит std::bad_alloc
        copy_buffer(str, length_);
    }

    SecureMessage::SecureMessage(const SecureMessage& other)
    {
        length_ = other.length_;
        if (length_ == 0 || !other.data_) {
            data_ = nullptr;
            return;
        }
        
        data_ = new char[length_ + 1];
        copy_buffer(other.data_, length_);
    }

    SecureMessage& SecureMessage::operator=(const SecureMessage& other)
    {
        if (this == &other) 
        {
            return *this;
        }
        
        if (other.length_ == 0 || !other.data_) 
        {
            delete[] data_;
            data_ = nullptr;
            length_ = 0;
            return *this;
        }
        
        // Строгая гарантия исключений: если new бросит исключение,
        // старые data_ и length_ не пострадают
        char* tmp = new char[other.length_ + 1];
        for (size_t i = 0; i <= other.length_; ++i) 
        {
            tmp[i] = other.data_[i];
        }
        
        delete[] data_;
        data_ = tmp;
        length_ = other.length_;
        
        return *this;
    }

    SecureMessage::SecureMessage(SecureMessage&& other) noexcept
        : data_(other.data_), length_(other.length_) 
    {
        other.data_ = nullptr;
        other.length_ = 0;
    }

    SecureMessage& SecureMessage::operator=(SecureMessage&& other) noexcept {
        if (this == &other) 
        {
            return *this;
        }
        delete[] data_;
        data_ = std::exchange(other.data_, nullptr);
        length_ = std::exchange(other.length_, 0);
        return *this;
    }

    SecureMessage::~SecureMessage() {
        delete[] data_;
    }

    const char* SecureMessage::get_data() const {
        return data_;
    }

    size_t SecureMessage::get_length() const {
        return length_;
    }


    bool SecureMessage::operator==(const SecureMessage& other) const
    {
        if (length_ != other.get_length())
        {
            return false;
        }

        for (int i = 0; i < length_; i++)
        {
            if (data_[i] != other.get_data()[i])
            {
                return false;
            }
        }
        
        return true;
    }

    SecureMessage SecureMessage::operator+(const SecureMessage& other) const
    {
        SecureMessage result;

        result.length_ = length_ + other.get_length();
        result.data_ = new char[result.length_ + 1];

        for (int i = 0; i < length_; i++)
        {
            result.data_[i] = data_[i];
        }

        for (int i = 0; i < other.get_length(); i++)
        {
            result.data_[i + length_] = other.data_[i];
        }
        result.data_[result.length_] = '\0';

        return result;
    }

    SecureMessage& SecureMessage::operator+=(const SecureMessage& other)
    {
        char * tmp = new char[length_ + other.length_ + 1];
        for (size_t i = 0; i < length_; i++)
        {
            tmp[i] = data_[i];
        }

        for (size_t i = 0; i < other.length_; i++)
        {
            tmp[i + length_] = other.data_[i];
        }
        tmp[length_ + other.length_] = '\0';
        delete [] data_;
        data_ = tmp;
        length_ += other.length_;

        return *this;
    }

    char& SecureMessage::operator[](size_t index)
    {
        if (index >= length_)
        {
            throw std::out_of_range("Index out of range");
        }

        return data_[index];
    }

    const char& SecureMessage::operator[](size_t index) const
    {
        if (index >= length_)
        {
            throw std::out_of_range("Index out of range");
        }

        return data_[index];
    }

    std::ostream& operator<<(std::ostream& os, const SecureMessage& msg)
    {
        if (msg.get_data() != nullptr)
        {
            os << msg.get_data();
        }
        return os;
    }
}
