#include <cstddef>
#include <ostream>

namespace context7 {

    class SecureMessage {
    private:
        char*  data_;
        size_t length_;
        size_t compute_length(const char* str);
        
        void copy_buffer(const char* src, size_t len);
        char& op_index(size_t index) const;
    public:
        SecureMessage();
        SecureMessage(const char* str);
        SecureMessage(const SecureMessage& other);
        SecureMessage& operator=(const SecureMessage& other);
        SecureMessage(SecureMessage&& other) noexcept;
        SecureMessage& operator=(SecureMessage&& other) noexcept;
        ~SecureMessage();

        bool operator==(const SecureMessage& other) const;
        SecureMessage operator+(const SecureMessage& other) const;
        SecureMessage& operator+=(const SecureMessage& other);
        char& operator[](size_t index);
        const char& operator [](size_t index) const;
        friend std::ostream& operator<<(std::ostream& os, const SecureMessage& msg);

        const char* get_data() const;
        size_t      get_length() const;
    };
}