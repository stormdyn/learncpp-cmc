# CPP-task for training

---

## Лекции 1–4: Классы, RAII, Полиморфизм, Перегрузка операторов

### Task 1: Класс `SecureMessage`

Написать класс `SecureMessage` в ООП-стиле на C++23. Весь код должен быть внутри `namespace context7`. Помни про ограничение: функции не больше 25 строк. Дублирующийся код (например, копирование буфера) выноси в приватные вспомогательные методы.

#### Требования:

- Приватные поля: `char* data_` (динамический массив) и `size_t length_`.
- Написать **конструктор по умолчанию**, инициализирующий пустой объект (`nullptr` и `0`).
- Написать **конструктор от `const char*`**, выделяющий память и копирующий переданную C-строку. В случае `nullptr` на входе создавать пустой объект.
- Написать **копирующий конструктор** (Copy Constructor): глубокое копирование.
- Написать **оператор копирующего присваивания** (Copy Assignment): с проверкой на самоприсваивание и освобождением старой памяти.
- Написать **перемещающий конструктор** (Move Constructor): забирает ресурс и оставляет `other` в валидном пустом состоянии (`noexcept`).
- Написать **оператор перемещающего присваивания** (Move Assignment): `noexcept`, с проверкой на самоприсваивание.
- Написать **деструктор**, очищающий выделенную память.
- Написать два метода-геттера: `const char* get_data() const` и `size_t get_length() const`.
- Использовать квалификатор `const` для всех переменных и параметров, где не подразумевается изменение.
- Для выделения памяти использовать обычный `new` (без `std::nothrow`).
- Ошибки выделения памяти **не обрабатывать** явно: при `std::bad_alloc` допускается штатное завершение.
- В операторах присваивания сохранять строгую гарантию исключений.

#### Структура файлов:

```
secure_message.hpp  — объявления класса
secure_message.cpp  — реализация методов
```

---

### Task 2: Иерархия шифровщиков сообщений (`MessageCipher`)

Создать полиморфную иерархию классов для шифрования сообщений в `namespace context7`. Задача демонстрирует работу с виртуальными функциями, абстрактными классами и наследованием.

#### Задача 1: Абстрактный базовый класс `MessageCipher`

- Класс должен быть **абстрактным** (нельзя создать экземпляр напрямую).
- Написать два чисто виртуальных метода:
  ```cpp
  virtual SecureMessage encrypt(const SecureMessage& plain) const = 0;
  virtual SecureMessage decrypt(const SecureMessage& cipher) const = 0;
  ```
- Написать виртуальный деструктор: `virtual ~MessageCipher() = default;`

#### Задача 2: Класс `CaesarCipher` (шифр Цезаря)

- Наследуется от `MessageCipher`.
- Приватное поле `int shift_`.
- `explicit CaesarCipher(int shift)`.
- `encrypt`: каждый символ сдвигается на `shift_` позиций по ASCII.
- `decrypt`: обратный сдвиг.

#### Задача 3: Класс `XorCipher` (XOR-шифр)

- Наследуется от `MessageCipher`.
- Приватное поле `char key_`.
- `explicit XorCipher(char key)`.
- `encrypt`/`decrypt`: каждый символ XOR с `key_` (операция идемпотентна).

#### Структура файлов:

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

#### Общие требования к Task 2:

- Функции не более 25 строк, `const`-корректность, `override`, `explicit`.
- Тест: `decrypt(encrypt(msg))` должен вернуть исходное сообщение.
- Проверить на утечки памяти через `valgrind`.

#### Пример использования:

```cpp
#include "message_cipher.hpp"
#include <iostream>

int main() {
    using namespace context7;

    SecureMessage original("Hello, World!");

    MessageCipher* cipher1 = new CaesarCipher(3);
    MessageCipher* cipher2 = new XorCipher('K');

    SecureMessage enc1 = cipher1->encrypt(original);
    SecureMessage dec1 = cipher1->decrypt(enc1);

    SecureMessage enc2 = cipher2->encrypt(original);
    SecureMessage dec2 = cipher2->decrypt(enc2);

    std::cout << "Original:         " << original.get_data() << "\n";
    std::cout << "Caesar encrypted: " << enc1.get_data() << "\n";
    std::cout << "Caesar decrypted: " << dec1.get_data() << "\n";
    std::cout << "XOR encrypted:    " << enc2.get_data() << "\n";
    std::cout << "XOR decrypted:    " << dec2.get_data() << "\n";

    delete cipher1;
    delete cipher2;
    return 0;
}
```

---

### Task 3: Перегрузка операторов для `SecureMessage`

Развиваем класс `SecureMessage` из Task 1, добавляя интуитивный интерфейс через операторы. Все изменения вносить в `secure_message.hpp` и `secure_message.cpp`.

#### Требования:

- Подключить `<stdexcept>` и `<iostream>`.
- **`operator==`** `bool operator==(const SecureMessage& other) const` — сравнение по содержимому (сначала длины, затем посимвольно). `operator!=` компилятор сгенерирует автоматически (C++20/23).
- **`operator+`** `SecureMessage operator+(const SecureMessage& other) const` — конкатенация двух сообщений; память выделяется ровно один раз.
- **`operator+=`** `SecureMessage& operator+=(const SecureMessage& other)` — дописывает `other` в конец; строгая гарантия исключений (сначала выделить новый буфер, затем удалить старый).
- **`operator[]`** — `char& operator[](size_t index)` и `const`-версия; при `index >= length_` бросать `std::out_of_range("Index out of range")`.
- **`operator<<`** — дружественная функция вывода в поток: `friend std::ostream& operator<<(std::ostream& os, const SecureMessage& msg)`.

---

## Лекция 5: Исключения, RTTI, Шаблоны

### Task 4: Шаблонный стек с исключениями и RTTI

Реализовать шаблонный контейнер `Stack<T, MaxSize>` в `namespace context7` с полноценной обработкой исключений и применением RTTI. Задача охватывает три ключевые темы лекции 5: шаблоны классов/функций, иерархию исключений и `dynamic_cast`/`typeid`.

#### Часть А: Иерархия исключений `StackError`

Определить собственную иерархию исключений, наследующуюся от `std::exception`:

```cpp
// stack_errors.hpp
#pragma once
#include <exception>
#include <string>

namespace context7 {

class StackError : public std::exception {
protected:
    std::string msg_;
public:
    explicit StackError(const std::string& msg) : msg_(msg) {}
    const char* what() const noexcept override { return msg_.c_str(); }
    virtual ~StackError() = default;
};

class StackOverflow : public StackError {
public:
    explicit StackOverflow()
        : StackError("Stack overflow: cannot push to a full stack") {}
};

class StackUnderflow : public StackError {
public:
    explicit StackUnderflow()
        : StackError("Stack underflow: cannot pop from an empty stack") {}
};

} // namespace context7
```

#### Часть Б: Шаблонный класс `Stack<T, MaxSize>`

Требования к классу:

- Шаблонные параметры: `typename T` и `int MaxSize`.
- Приватные поля: `T data_[MaxSize]` и `int top_` (индекс следующей свободной позиции, начальное значение `0`).
- Методы:
  - `void push(const T& value)` — добавляет элемент на вершину; бросает `StackOverflow` если `top_ == MaxSize`.
  - `T pop()` — снимает и возвращает вершину; бросает `StackUnderflow` если стек пуст.
  - `const T& peek() const` — возвращает вершину без снятия; бросает `StackUnderflow` если пуст.
  - `bool is_empty() const noexcept`
  - `bool is_full() const noexcept`
  - `int size() const noexcept`

```cpp
// stack.hpp
#pragma once
#include "stack_errors.hpp"

namespace context7 {

template <typename T, int MaxSize>
class Stack {
private:
    T data_[MaxSize];
    int top_ = 0;
public:
    void push(const T& value);
    T pop();
    const T& peek() const;
    bool is_empty() const noexcept { return top_ == 0; }
    bool is_full()  const noexcept { return top_ == MaxSize; }
    int  size()     const noexcept { return top_; }
};

} // namespace context7

#include "stack_impl.hpp"  // реализация шаблона — в отдельном .hpp
```

> **Примечание:** реализацию методов шаблонного класса помещать в `stack_impl.hpp`, который включается в конце `stack.hpp` (стандартная практика для template definitions).

#### Часть В: Шаблонная функция `print_stack_info`

Написать шаблонную функцию, которая принимает стек **по ссылке на базовый полиморфный интерфейс** — но сначала нужно добавить абстрактный интерфейс:

```cpp
// Добавить в stack.hpp перед Stack<T,MaxSize>:
class IStack {
public:
    virtual int  size()     const noexcept = 0;
    virtual bool is_empty() const noexcept = 0;
    virtual bool is_full()  const noexcept = 0;
    virtual ~IStack() = default;
};
```

Сделать `Stack<T, MaxSize>` наследником `IStack`. Затем написать функцию:

```cpp
template <typename T, int MaxSize>
void print_stack_info(const Stack<T, MaxSize>& s);
```

Функция должна:
1. Вывести `typeid(T).name()` — тип элементов стека.
2. Вывести `size()`, `is_empty()`, `is_full()`.
3. Принять `IStack*` (указатель на базовый класс) и через `dynamic_cast<const Stack<T, MaxSize>*>` проверить корректность типа — вывести `"[RTTI OK]"` или `"[RTTI FAIL]"`.

#### Часть Г: `main.cpp` — демонстрация и обработка ошибок

Написать `main.cpp`, демонстрирующий:

```cpp
#include "stack.hpp"
#include <iostream>

int main() {
    using namespace context7;

    Stack<int, 5> int_stack;
    print_stack_info<int, 5>(int_stack);

    // Заполнить стек
    for (int i = 1; i <= 5; ++i)
        int_stack.push(i * 10);

    // Попытка переполнения
    try {
        int_stack.push(999);
    } catch (const StackOverflow& e) {
        std::cerr << "[StackOverflow] " << e.what() << "\n";
    } catch (const StackError& e) {
        std::cerr << "[StackError]    " << e.what() << "\n";
    }

    // Выгрузить весь стек
    while (!int_stack.is_empty())
        std::cout << int_stack.pop() << " ";
    std::cout << "\n";

    // Попытка pop на пустом стеке
    try {
        int_stack.pop();
    } catch (const StackUnderflow& e) {
        std::cerr << "[StackUnderflow] " << e.what() << "\n";
    }

    // Стек строк
    Stack<const char*, 3> str_stack;
    str_stack.push("hello");
    str_stack.push("world");
    print_stack_info<const char*, 3>(str_stack);

    return 0;
}
```

#### Структура файлов:

```
stack_errors.hpp   — иерархия исключений
stack.hpp          — интерфейс IStack + объявление Stack<T,N>
stack_impl.hpp     — реализация методов шаблона
main.cpp           — демонстрация
```

#### Общие требования к Task 4:

- **Стандарт:** C++23.
- **Функции** не более 25 строк.
- `const`-корректность и `noexcept` там, где уместно.
- Исключения наследовать от `std::exception` и переопределять `what()`.
- Демонстрировать `catch` по иерархии: сначала производный тип, затем базовый (`StackOverflow` → `StackError` → `std::exception`).
- Использовать `typeid` и `dynamic_cast` в `print_stack_info`.
- Проверить отсутствие утечек памяти через `valgrind` (стек на массиве — ручного `new` нет, но проверить всё равно).

---

## Общие требования ко всем задачам

- **Стандарт:** C++23
- **Стиль:** объектно-ориентированный, `namespace context7`
- **Ограничения:**
  - Функции не более 25 строк (обоснованные исключения допустимы)
  - Повторяющийся код выносить в отдельные функции
  - `const` везде, где возможно
- **Управление памятью:**
  - Ручное управление через `new`/`delete` там, где требуется (учебная цель)
  - Rule of Five, RAII, корректная move-семантика
  - Явных проверок на ошибку выделения не делать — доверяем `std::bad_alloc`
