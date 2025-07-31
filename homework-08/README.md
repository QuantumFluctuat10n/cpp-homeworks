# Домашнє завдання ДЗ 8. Статичні бібліотеки

## Опис завдання
Створити статичну бібліотеку `MathLib` з математичними функціями та написати програму-калькулятор що її використовує.

## Файли
 - `MathCalculator.cpp` - Файл тестової програми з використанням всіх функцій бібліотеки
 - `MathLib.lib` - Скомпільований файл бібліотеки
 - `MathLib/`
    - `MathLib.cpp` - Реалізація всіх функцій бібліотеки
    - `MathLib.h` - Заголовочний файл з оголошеннями функцій бібліотеки
## Компіляція та запуск
```bash
g++ -c ./MathLib/MathLib.cpp -o MathLib.obj
ar rcs MathLib.lib MathLib.obj
g++ MathCalculator.cpp -L. -lMathLib -o MathCalculator
./MathCalculator.exe
```