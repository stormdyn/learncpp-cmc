#include <cstddef>

namespace context7 {

    class SecureMessage {
    private:
        char*  data_;
        size_t length_;

    public:
        SecureMessage();
        SecureMessage(const char* str);
        SecureMessage(const SecureMessage& other);
        SecureMessage& operator=(const SecureMessage& other);
        SecureMessage(SecureMessage&& other) noexcept;
        SecureMessage& operator=(SecureMessage&& other) noexcept;
        ~SecureMessage();

        const char* get_data() const;
        size_t      get_length() const;
    };
}