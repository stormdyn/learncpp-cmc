#include <utility>
#include "task1_interface.hpp"

namespace context7
{
    // Приватный вспомогательный метод для копирования буфера
    void SecureMessage::copy_buffer(const char* src, size_t len) {
        for (size_t i = 0; i < len; ++i) {
            data_[i] = src[i];
        }
        data_[len] = '\0';
    }

    // Приватный вспомогательный метод для вычисления длины строки
    size_t SecureMessage::compute_length(const char* str) {
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

    SecureMessage::SecureMessage(const char* str) {
        if (!str) {
            data_ = nullptr;
            length_ = 0;
            return;
        }
        
        length_ = compute_length(str);
        data_ = new char[length_ + 1];
        copy_buffer(str, length_);
    }

    SecureMessage::SecureMessage(const SecureMessage& other) {
        length_ = other.length_;
        if (length_ == 0) {
            data_ = nullptr;
            return;
        }
        data_ = new char[length_ + 1];
        copy_buffer(other.data_, length_);
    }

    SecureMessage& SecureMessage::operator=(const SecureMessage& other) {
        if (this == &other) {
            return *this;
        }
        
        if (other.length_ == 0) {
            delete[] data_;
            data_ = nullptr;
            length_ = 0;
            return *this;
        }
        
        char* tmp = new char[other.length_ + 1];
        for (size_t i = 0; i <= other.length_; ++i) {
            tmp[i] = other.data_[i];
        }
        
        delete[] data_;
        data_ = tmp;
        length_ = other.length_;
        return *this;
    }

    SecureMessage::SecureMessage(SecureMessage&& other) noexcept
        : data_(other.data_), length_(other.length_) {
        other.data_ = nullptr;
        other.length_ = 0;
    }

    SecureMessage& SecureMessage::operator=(SecureMessage&& other) noexcept {
        if (this == &other) {
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
}
