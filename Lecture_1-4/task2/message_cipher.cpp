#include "message_cipher.hpp"
#include <cstddef>

namespace context7
{
    CaesarCipher::CaesarCipher(int shift) : shift_(shift) {}

    SecureMessage CaesarCipher::shift_func(const SecureMessage& text, const int shift) const
    {
        const char * data = text.get_data();
        size_t length = text.get_length();
        if (length == 0 || !data) return SecureMessage();

        char * buffer = new char[length + 1];
        for (size_t i = 0; i < length; i++)
        {
            buffer[i] = shift + data[i];
        }

        buffer[length] = '\0';
        SecureMessage result(buffer);

        delete[] buffer;
        return result;
    }

    SecureMessage CaesarCipher::encrypt(const SecureMessage& plain) const
    {
        return shift_func(plain, shift_);
    }
    
    SecureMessage CaesarCipher::decrypt(const SecureMessage& cipher) const
    {
        return shift_func(cipher, -shift_);
    }


    XorCipher::XorCipher(char key) : key_(key) {}

    SecureMessage XorCipher::xor_func(const SecureMessage& text) const
    {
        const char * data = text.get_data();
        size_t length = text.get_length();
        if (length == 0 || !data) return SecureMessage();

        char * buffer = new char[length + 1];
        for (size_t i = 0; i < length; i++)
        {
            buffer[i] = key_ ^ data[i];
        }

        buffer[length] = '\0';
        SecureMessage result(buffer);

        delete[] buffer;
        return result;
    }

    SecureMessage XorCipher::encrypt(const SecureMessage& plain) const
    {
        return xor_func(plain);
    }

    SecureMessage XorCipher::decrypt(const SecureMessage& cipher) const
    {
        return xor_func(cipher);
    }
}