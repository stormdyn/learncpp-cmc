# CPP-task for training

## Task 1: Класс SecureMessage

Твоя задача: Класс SecureMessage

Напиши класс SecureMessage в ООП-стиле на C++23. Весь код должен быть внутри namespace context7. Помни про ограничение: функции не больше 25 строк. Дублирующийся код (например, копирование буфера) выноси в приватные вспомогательные методы.

### Требования к классу:

1. Приватные поля: `char* data_` (динамический массив) и `size_t length_`.
2. **Конструктор по умолчанию**: инициализирует пустой объект (`nullptr` и `0`).
3. **Конструктор от `const char*`**: выделяет память и копирует переданную C-строку.
4. **Копирующий конструктор** (Copy Constructor): глубокое копирование.
5. **Оператор копирующего присваивания** (Copy Assignment): с проверкой на самоприсваивание и освобождением старой памяти.
6. **Перемещающий конструктор** (Move Constructor): забирает ресурс и оставляет `other` в валидном пустом состоянии (`noexcept`).
7. **Оператор перемещающего присваивания** (Move Assignment): `noexcept`, с проверкой на самоприсваивание.
8. **Деструктор**: очищает выделенную память.
9. Пару методов-геттеров для удобства: `const char* get_data() const` и `size_t get_length() const`

---

## Task 2: Иерархия шифровщиков сообщений (MessageCipher)

Создай полиморфную иерархию классов для шифрования сообщений в `namespace context7`. Задача демонстрирует работу с виртуальными функциями, абстрактными классами и наследованием.

### Пункт 1: Абстрактный базовый класс `MessageCipher`

**Требования:**
- Класс должен быть **абстрактным** (нельзя создать экземпляр напрямую).
- Два чисто виртуальных метода:
  ```cpp
  virtual SecureMessage encrypt(const SecureMessage& plain) const = 0;
  virtual SecureMessage decrypt(const SecureMessage& cipher) const = 0;
  ```
- Виртуальный деструктор:
  ```cpp
  virtual ~MessageCipher() = default;
  ```
- Методы помечены `const`, так как не меняют состояние объекта-шифратора.

### Пункт 2: Класс `CaesarCipher` (шифр Цезаря)

**Требования:**
- Наследуется от `MessageCipher`: `class CaesarCipher : public MessageCipher`
- Приватное поле:
  ```cpp
  int shift_;  // величина сдвига
  ```
- Конструктор:
  ```cpp
  explicit CaesarCipher(int shift);
  ```
- Переопределённые методы (с `override`):
  ```cpp
  SecureMessage encrypt(const SecureMessage& plain) const override;
  SecureMessage decrypt(const SecureMessage& cipher) const override;
  ```

**Алгоритм:**
- `encrypt`: каждый символ сдвигается на `shift_` позиций по ASCII: `c + shift_`
- `decrypt`: обратный сдвиг на `-shift_`
- Возвращается новый объект `SecureMessage`, исходный не изменяется

### Пункт 3: Класс `XorCipher` (симметричный XOR-шифр)

**Требования:**
- Наследуется от `MessageCipher`: `class XorCipher : public MessageCipher`
- Приватное поле:
  ```cpp
  char key_;  // байт-ключ для XOR
  ```
- Конструктор:
  ```cpp
  explicit XorCipher(char key);
  ```
- Переопределённые методы (с `override`):
  ```cpp
  SecureMessage encrypt(const SecureMessage& plain) const override;
  SecureMessage decrypt(const SecureMessage& cipher) const override;
  ```

**Алгоритм:**
- `encrypt`: каждый символ XOR с ключом: `c ^ key_`
- `decrypt`: та же операция (XOR идемпотентен: `(c ^ k) ^ k = c`)
- Возвращается новый объект `SecureMessage`

### Пункт 4: Структура проекта

**Файлы:**
```
secure_message.hpp / secure_message.cpp  (из Task 1)
message_cipher.hpp / message_cipher.cpp  (новые файлы)
```

**Заголовок `message_cipher.hpp`:**
```cpp
#pragma once
#include "secure_message.hpp"

namespace context7 {

class MessageCipher {
public:
    virtual SecureMessage encrypt(const SecureMessage& plain) const = 0;
    virtual SecureMessage decrypt(const SecureMessage& cipher) const = 0;
    virtual ~MessageCipher() = default;
};

class CaesarCipher : public MessageCipher {
private:
    int shift_;
public:
    explicit CaesarCipher(int shift);
    SecureMessage encrypt(const SecureMessage& plain) const override;
    SecureMessage decrypt(const SecureMessage& cipher) const override;
};

class XorCipher : public MessageCipher {
private:
    char key_;
public:
    explicit XorCipher(char key);
    SecureMessage encrypt(const SecureMessage& plain) const override;
    SecureMessage decrypt(const SecureMessage& cipher) const override;
};

} // namespace context7
```

### Пункт 5: Требования к реализации

1. **Стиль кода:**
   - Разделение интерфейса и реализации (объявления в `.hpp`, определения в `.cpp`)
   - Функции не более 25 строк, дублирующийся код — в приватные вспомогательные методы
   - `const`-корректность везде, где возможно
   - Использовать `override` для виртуальных методов
   - Использовать `explicit` для конструкторов от одного аргумента

2. **Алгоритм методов `encrypt/decrypt`:**
   - Получить данные и длину из `SecureMessage` через геттеры
   - Создать временный буфер `char* buffer = new char[length + 1]`
   - Применить преобразование (сдвиг или XOR) в цикле
   - Установить завершающий ноль: `buffer[length] = '\0'`
   - Создать результат: `SecureMessage result(buffer)`
   - Освободить временный буфер: `delete[] buffer`
   - Вернуть результат (сработает RVO/move-семантика)

3. **Проверка корректности:**
   - Тест: `decrypt(encrypt(msg))` должен вернуть исходное сообщение
   - Никаких утечек памяти (проверить через valgrind)
   - Полиморфный вызов через указатель `MessageCipher*` должен корректно работать

### Пункт 6: Пример использования (для будущего `main.cpp`)

```cpp
#include "message_cipher.hpp"
#include <iostream>

int main() {
    using namespace context7;
    
    SecureMessage original("Hello, World!");
    
    // Полиморфизм через базовый класс
    MessageCipher* cipher1 = new CaesarCipher(3);
    MessageCipher* cipher2 = new XorCipher('K');
    
    SecureMessage encrypted1 = cipher1->encrypt(original);
    SecureMessage decrypted1 = cipher1->decrypt(encrypted1);
    
    SecureMessage encrypted2 = cipher2->encrypt(original);
    SecureMessage decrypted2 = cipher2->decrypt(encrypted2);
    
    std::cout << "Original: " << original.get_data() << std::endl;
    std::cout << "Caesar encrypted: " << encrypted1.get_data() << std::endl;
    std::cout << "Caesar decrypted: " << decrypted1.get_data() << std::endl;
    std::cout << "XOR encrypted: " << encrypted2.get_data() << std::endl;
    std::cout << "XOR decrypted: " << decrypted2.get_data() << std::endl;
    
    delete cipher1;
    delete cipher2;
    
    return 0;
}
```

---

## Общие требования ко всем задачам

- **Стандарт:** C++23
- **Стиль:** Объектно-ориентированный
- **Пространство имён:** `context7`
- **Ограничения:**
  - Функции не более 25 строк (обоснованные исключения допустимы)
  - Повторяющийся код выносить в отдельные функции
  - Использовать `const` везде, где возможно
- **Управление памятью:**
  - Ручное управление через `new`/`delete` (учебная цель)
  - Соблюдение правила пяти (Rule of Five)
  - RAII и корректная семантика перемещения
