# CPP-task for training

## Task 1: Класс `SecureMessage`

Написать класс `SecureMessage` в ООП-стиле на C++23. Весь код должен быть внутри `namespace context7`. Помни про ограничение: функции не больше 25 строк. Дублирующийся код (например, копирование буфера) выноси в приватные вспомогательные методы.

### Требования:

- Приватные поля: `char* data_` (динамический массив) и `size_t length_`.
- Написать **конструктор по умолчанию**, инициализирующий пустой объект (`nullptr` и `0`).
- Написать **конструктор от `const char*`**, выделяющий память и копирующий переданную C-строку. В случае `nullptr` на входе создавать пустой объект.
- Написать **копирующий конструктор** (Copy Constructor): глубокое копирование.
- Написать **оператор копирующего присваивания** (Copy Assignment): с проверкой на самоприсваивание и освобождением старой памяти.
- Написать **перемещающий конструктор** (Move Constructor): забирает ресурс и оставляет `other` в валидном пустом состоянии (`noexcept`).
- Написать **оператор перемещающего присваивания** (Move Assignment): `noexcept`, с проверкой на самоприсваивание.
- Написать **деструктор**, очищающий выделенную память.
- Написать два метода-геттера для удобства: `const char* get_data() const` и `size_t get_length() const`
- Использовать квалификатор `const` для всех переменных и параметров, где не подразумевается изменение соответствующей области памяти.
- Для выделения памяти использовать обычный `new` (без `std::nothrow`).
- Ошибки выделения памяти **не обрабатывать** явно: при возникновении `std::bad_alloc` допускается штатное аварийное завершение программы (`terminate`).
- В операторах присваивания сохранять строгую гарантию исключений: при выброшенном `std::bad_alloc` объект должен остаться в прежнем валидном состоянии (сначала выделять новую память, затем удалять старую).
- По завершении работы вся динамически выделенная память должна быть освобождена деструктором.

### Структура файлов:

```
secure_message.hpp  — объявления класса
secure_message.cpp  — реализация методов
```

---

## Task 2: Иерархия шифровщиков сообщений (MessageCipher)

Создать полиморфную иерархию классов для шифрования сообщений в `namespace context7`. Задача демонстрирует работу с виртуальными функциями, абстрактными классами и наследованием.

### Задача 1: Абстрактный базовый класс `MessageCipher`

**Требования:**
- Класс должен быть **абстрактным** (нельзя создать экземпляр напрямую).
- Написать два чисто виртуальных метода:
  ```cpp
  virtual SecureMessage encrypt(const SecureMessage& plain) const = 0;
  virtual SecureMessage decrypt(const SecureMessage& cipher) const = 0;
  ```
- Написать виртуальный деструктор:
  ```cpp
  virtual ~MessageCipher() = default;
  ```
- Методы помечены `const`, так как не меняют состояние объекта-шифратора.

### Задача 2: Класс `CaesarCipher` (шифр Цезаря)

**Требования:**
- Наследуется от `MessageCipher`: `class CaesarCipher : public MessageCipher`
- Написать приватное поле:
  ```cpp
  int shift_;  // величина сдвига
  ```
- Написать конструктор:
  ```cpp
  explicit CaesarCipher(int shift);
  ```
- Написать переопределённые методы (с `override`):
  ```cpp
  SecureMessage encrypt(const SecureMessage& plain) const override;
  SecureMessage decrypt(const SecureMessage& cipher) const override;
  ```

**Алгоритм:**
- `encrypt`: каждый символ сдвигается на `shift_` позиций по ASCII: `c + shift_`
- `decrypt`: обратный сдвиг на `-shift_`
- Возвращается новый объект `SecureMessage`, исходный не изменяется

**Идея реализации:**
- Получить данные из `SecureMessage` через геттеры `get_data()` и `get_length()`.
- Создать временный буфер `char* buffer = new char[length + 1]`.
- Применить преобразование (сдвиг) в цикле.
- Установить завершающий ноль: `buffer[length] = '\0'`.
- Создать результат: `SecureMessage result(buffer)`.
- Освободить временный буфер: `delete[] buffer`.
- Вернуть результат (сработает RVO/move-семантика).

### Задача 3: Класс `XorCipher` (симметричный XOR-шифр)

**Требования:**
- Наследуется от `MessageCipher`: `class XorCipher : public MessageCipher`
- Написать приватное поле:
  ```cpp
  char key_;  // байт-ключ для XOR
  ```
- Написать конструктор:
  ```cpp
  explicit XorCipher(char key);
  ```
- Написать переопределённые методы (с `override`):
  ```cpp
  SecureMessage encrypt(const SecureMessage& plain) const override;
  SecureMessage decrypt(const SecureMessage& cipher) const override;
  ```

**Алгоритм:**
- `encrypt`: каждый символ XOR с ключом: `c ^ key_`
- `decrypt`: та же операция (XOR идемпотентен: `(c ^ k) ^ k = c`)
- Возвращается новый объект `SecureMessage`

**Идея реализации:**
- Аналогично `CaesarCipher`, но вместо сдвига делать `buffer[i] = plain.get_data()[i] ^ key_`.
- Остальное идентично.

### Структура файлов:

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

### Общие требования к Task 2:

- **Стиль кода:**
  - Разделение интерфейса и реализации (объявления в `.hpp`, определения в `.cpp`)
  - Функции не более 25 строк, дублирующийся код — в приватные вспомогательные методы
  - `const`-корректность везде, где возможно
  - Использовать `override` для виртуальных методов
  - Использовать `explicit` для конструкторов от одного аргумента
- **Проверка корректности:**
  - Тест: `decrypt(encrypt(msg))` должен вернуть исходное сообщение
  - Никаких утечек памяти (проверить через valgrind)
  - Полиморфный вызов через указатель `MessageCipher*` должен корректно работать
- **Управление памятью:**
  - Для выделения памяти использовать обычный `new` (без `std::nothrow`).
  - Ошибки выделения памяти **не обрабатывать** явно (допускается падение с `std::bad_alloc`).
  - По завершении работы вся динамически выделенная память должна быть освобождена деструктором.

### Пример использования (для будущего `main.cpp`):

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
  - Отсутствие явных проверок на ошибку выделения памяти (доверяем `std::bad_alloc`)