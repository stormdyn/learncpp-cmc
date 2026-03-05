#include "../task1/secure_message.hpp"
namespace context7 
{
    class MessageCipher
    {
        public:
            virtual SecureMessage encrypt(const SecureMessage& plain) const = 0;
            virtual SecureMessage decrypt(const SecureMessage& cipher) const = 0;
            virtual ~MessageCipher() = default;
    };

    class CaesarCipher : public MessageCipher
    {
        private:
            int shift_;
        public:
            explicit CaesarCipher(int shift);

            SecureMessage encrypt(const SecureMessage& plain) const override;
            SecureMessage decrypt(const SecureMessage& cipher) const override;
    };

    class XorCipher : public MessageCipher
    {
        private:
            char key_;
            
        public:
            explicit XorCipher(char key);
            SecureMessage encrypt(const SecureMessage& plain) const override;
            SecureMessage decrypt(const SecureMessage& cipher) const override;
    };
}