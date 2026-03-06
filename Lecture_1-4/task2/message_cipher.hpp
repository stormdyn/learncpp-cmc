#pragma once

#include "../task3/secure_message.hpp"
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
            SecureMessage shift_func(const SecureMessage& text, const int shift) const;

        public:
            explicit CaesarCipher(int shift);

            SecureMessage encrypt(const SecureMessage& plain) const override;
            SecureMessage decrypt(const SecureMessage& cipher) const override;
    };

    class XorCipher : public MessageCipher
    {
        private:
            char key_;
            SecureMessage xor_func(const SecureMessage& text) const;
            
        public:
            explicit XorCipher(char key);
            SecureMessage encrypt(const SecureMessage& plain) const override;
            SecureMessage decrypt(const SecureMessage& cipher) const override;
    };

}