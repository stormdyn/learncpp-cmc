#include "message_cipher.hpp"
#include <cstddef>

namespace context7
{
    CaesarCipher::CaesarCipher(int shift)
    {
        shift_ = shift;
    }

    SecureMessage CaesarCipher::encrypt(const SecureMessage& plain) const
    {
        const char * data = plain.get_data();
        size_t length = plain.get_length();
        char * buffer = new char[length + 1];

        for (int i = 0; i < length; i++)
        {
            buffer[i] = shift_ + data[i];
        }

        buffer[length] = '\0';
        SecureMessage cipher(buffer);

        delete[] buffer;
        return cipher;
    }
    
    SecureMessage CaesarCipher::decrypt(const SecureMessage& cipher) const
    {
        const char * data = cipher.get_data();
        size_t length = cipher.get_length();
        char * buffer = new char[length + 1];

        for (int i = 0; i < length; i++)
        {
            buffer[i] = data[i] - shift_;
        }

        buffer[length] = '\0';
        SecureMessage plain(buffer);

        delete[] buffer;
        return plain;
    }


    SecureMessage XorCipher::encrypt(const SecureMessage& plain) const
    {
        const char * data = plain.get_data();
        size_t length = plain.get_length();
        char * buffer = new char[length + 1];

        for (int i = 0; i < length; i++)
        {
            buffer[i] = key_ ^ data[i];
        }

        buffer[length] = '\0';
        SecureMessage cipher(buffer);

        delete[] buffer;
        return cipher;
    }

    SecureMessage XorCipher::decrypt(const SecureMessage& cipher) const
    {
        const char * data = cipher.get_data();
        size_t length = cipher.get_length();
        char * buffer = new char[length + 1];

        for (int i = 0; i < length; i++)
        {
            buffer[i] = key_ ^ data[i];
        }

        buffer[length] = '\0';
        SecureMessage plain(buffer);

        delete[] buffer;
        return plain;
    }
}