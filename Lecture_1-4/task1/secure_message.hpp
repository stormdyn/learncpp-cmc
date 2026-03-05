#include <cstddef>

namespace context7 {

    class SecureMessage {
    private:
        char*  data_;
        size_t length_;
        size_t compute_length(const char* str);
        
        void copy_buffer(const char* src, size_t len);
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