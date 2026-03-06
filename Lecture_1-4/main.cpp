#include <iostream>
#include <cassert>
#include <stdexcept>
#include "task3/secure_message.hpp"
#include "task2/message_cipher.hpp"

using namespace context7;

void test_rule_of_five() {
    SecureMessage msg1("Hello");
    SecureMessage msg2 = msg1; // Копирующий конструктор
    assert(msg1 == msg2);

    SecureMessage msg3;
    msg3 = msg1; // Копирующее присваивание
    assert(msg3 == msg1);

    SecureMessage msg4 = std::move(msg2); // Перемещающий конструктор
    assert(msg4 == msg1);
    assert(msg2.get_data() == nullptr); // Проверяем, что ресурс забрали

    SecureMessage msg5;
    msg5 = std::move(msg3); // Перемещающее присваивание
    assert(msg5 == msg1);
    assert(msg3.get_data() == nullptr);
    
    std::cout << "[+] Rule of Five tests passed.\n";
}

void test_operators() {
    SecureMessage msg1("Hello");
    SecureMessage msg2(", World!");

    // Тест оператора +
    SecureMessage msg3 = msg1 + msg2;
    assert(msg3 == SecureMessage("Hello, World!"));

    // Тест оператора +=
    msg1 += msg2;
    assert(msg1 == SecureMessage("Hello, World!"));

    // Тест оператора []
    assert(msg1[0] == 'H');
    msg1[0] = 'h';
    assert(msg1[0] == 'h');

    // Тест исключения out_of_range
    bool exception_thrown = false;
    try {
        char c = msg1[999]; // Явный выход за границы
    } catch (const std::out_of_range& e) {
        exception_thrown = true;
    }
    assert(exception_thrown && "Exception was not thrown for out of bounds!");

    std::cout << "[+] Operator tests passed.\n";
}

void test_polymorphism() {
    SecureMessage original("TopSecretData");

    // Тестируем шифр Цезаря через указатель на базовый класс
    MessageCipher* caesar = new CaesarCipher(7);
    SecureMessage caesar_enc = caesar->encrypt(original);
    SecureMessage caesar_dec = caesar->decrypt(caesar_enc);
    
    // Убеждаемся, что шифр меняет строку, а дешифровка возвращает оригинал
    assert(!(caesar_enc == original));
    assert(caesar_dec == original);
    delete caesar;

    // Тестируем XOR шифр
    MessageCipher* xor_cipher = new XorCipher('K');
    SecureMessage xor_enc = xor_cipher->encrypt(original);
    SecureMessage xor_dec = xor_cipher->decrypt(xor_enc);
    
    assert(!(xor_enc == original));
    assert(xor_dec == original);
    delete xor_cipher;

    std::cout << "[+] Polymorphism & Cipher tests passed.\n";
}

int main() {
    std::cout << "=== Running SecureMessage Tests ===\n";
    test_rule_of_five();
    test_operators();
    test_polymorphism();
    
    std::cout << "\nAll tests passed successfully!";
    return 0;
}
