#include "utility"
#include "task1_interface.hpp"

namespace context7
{
    SecureMessage::SecureMessage() : data_(nullptr), length_(0) {}

    SecureMessage::SecureMessage(const char* str) {
        size_t len_tmp = 0;
        while (str[len_tmp] != '\0') {
            ++len_tmp;
        }
        length_ = len_tmp;
        data_   = new char[length_ + 1];
        for (size_t i = 0; i < length_; ++i) {
            data_[i] = str[i];
        }
        data_[length_] = '\0';
    }

    SecureMessage::SecureMessage(const SecureMessage& other) {
        length_ = other.length_;
        data_   = new char[length_ + 1];
        for (size_t i = 0; i < length_; ++i) {
            data_[i] = other.data_[i];
        }
        data_[length_] = '\0';
    }

    SecureMessage& SecureMessage::operator=(const SecureMessage& other) {
        if (this == &other) {
            return *this;
        }
        char* tmp = new char[other.length_ + 1];
        for (size_t i = 0; i < other.length_; ++i) {
            tmp[i] = other.data_[i];
        }
        tmp[other.length_] = '\0';

        delete[] data_;
        data_   = tmp;
        length_ = other.length_;
        return *this;
    }

    SecureMessage::SecureMessage(SecureMessage&& other) noexcept
        : data_(other.data_), length_(other.length_) {
        other.data_   = nullptr;
        other.length_ = 0;
    }

    SecureMessage& SecureMessage::operator=(SecureMessage&& other) noexcept {
        if (this == &other) {
            return *this;
        }
        delete[] data_;
        data_   = std::exchange(other.data_, nullptr);
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