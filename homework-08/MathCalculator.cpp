#include "MathLib/MathLib.h"
#include <iostream>
#include <stdexcept>
#include <vector>
using namespace std;

void showMenu() {
    cout << "=== MATHLIB TEST PROGRAM ===\n";
    cout << "1. Basic operations\n";
    cout << "2. Math functions\n";
    cout << "3. Arrays\n";
    cout << "4. Integers\n";
    cout << "5. Demo Mode\n";
    cout << "0. Exit\n";
    cout << "Choose option: ";
}

void BasicOperations(){
    double a, b;
    cout << "\nEnter first value: ";
    cin >> a;
    cout << "Enter second value: ";
    cin >> b;

    cout << "\n=== Basic operations result ===\n";
    cout << a << " + " << b << " = " << add(a, b) << '\n';
    cout << a << " - " << b << " = " << subtract(a, b) << '\n';
    cout << a << " * " << b << " = " << multiply(a, b) << '\n';
    cout << a << " / " << b << " = " << divide(a, b) << '\n';
}

void MathFunctions() {
    double base, rootValue, absValue;
    int exponent;

    cout << "\nPower function\nEnter base: ";
    cin >> base;
    cout << "Enter exponent: ";
    cin >> exponent;

    cout << "\nAbsolute value function\nEnter value: ";
    cin >> absValue;

    cout << "\nSquare root function\nEnter value: ";
    cin >> rootValue;

    cout << "\n=== Math functions result ===\n";
    cout << "power(" << base << ", " << exponent << ") = " << power(base, exponent) << '\n';
    cout << "absoluteValue(" << absValue << ") = " << absoluteValue(absValue) << '\n';
    cout << "squareRoot(" << rootValue << ") = " << squareRoot(rootValue) << '\n';
}

void Array() {
    int size;
    cout << "\nEnter array size: ";
    cin >> size;

    vector<double> arr(size);
    cout << "Enter " << size << " values:\n";
    for (double &val : arr) cin >> val;

    cout << "\n=== Arrays result ===\n";
    cout << "Highest value: " << findMax(arr.data(), size) << '\n';
    cout << "Lowest value: " << findMin(arr.data(), size) << '\n';
    cout << "Sum of values: " << calculateSum(arr.data(), size) << '\n';
    cout << "Average value: " << calculateAverage(arr.data(), size) << '\n';

    sortArray(arr.data(), size);
    cout << "Sorted array:";
    for (double val : arr) cout << " " << val;
    cout << '\n';
}

void Integers() {
    int factorial_, isPrime_, gcdA, gcdB, fibonacci_;
    cout << "\nFactorial function\nEnter a number: ";
    cin >> factorial_;
    cout << "\nIs prime function\nEnter a number: ";
    cin >> isPrime_;
    cout << "\nGCD function\nEnter first number: ";
    cin >> gcdA;
    cout << "Enter second number: ";
    cin >> gcdB;
    cout << "\nFibonacci function\nEnter a number: ";
    cin >> fibonacci_;

    cout << "\n=== Integers result ===\n";
    cout << "factorial(" << factorial_ << ") = " << factorial(factorial_) << endl;
    cout << "isPrime(" << isPrime_ << ") = " << (isPrime(isPrime_) ? "true" : "false") << endl;
    cout << "gcd(" << gcdA << ", " << gcdB << ") = " << gcd(gcdA, gcdB) << endl;
    cout << "fibonacci(" << fibonacci_ << ") = " << fibonacci(fibonacci_) << endl;
}

void demoMode() {
    cout << "\n=== DEMO MODE ===\n";

    double a = 256, b = 64;
    cout << "\n=== Basic operations ===\n";
    cout << a << " + " << b << " = " << add(a, b) << '\n';
    cout << a << " - " << b << " = " << subtract(a, b) << '\n';
    cout << a << " * " << b << " = " << multiply(a, b) << '\n';
    cout << a << " / " << b << " = " << divide(a, b) << '\n';

    double base = 3, rootValue = 2, absValue = -27.6;
    int exponent = -3;
    cout << "\n=== Math functions ===\n";
    cout << "power(" << base << ", " << exponent << ") = " << power(base, exponent) << '\n';
    cout << "absoluteValue(" << absValue << ") = " << absoluteValue(absValue) << '\n';
    cout << "squareRoot(" << rootValue << ") = " << squareRoot(rootValue) << '\n';

    double arr[] = {5.5, 9.1, 2.2, 15.0, 0.3};
    int size = sizeof(arr) / sizeof(arr[0]);

    cout << "\n=== Array operations ===\nArray values: ";
    for (double val : arr) cout << val << " ";
    cout << endl;
    cout << "Highest value: " << findMax(arr, size) << '\n';
    cout << "Lowest value: " << findMin(arr, size) << '\n';
    cout << "Sum of values: " << calculateSum(arr, size) << '\n';
    cout << "Average value: " << calculateAverage(arr, size) << '\n';

    sortArray(arr, size);
    cout << "Sorted array:";
    for (int i = 0; i < size; i++) cout << " " << arr[i];
    cout << endl;

    int factorialVal = 5, primeVal = 17, gcdA = 36, gcdB = 60, fibonacciVal = 9;
    cout << "\n=== Integer functions ===\n";
    cout << "factorial(" << factorialVal << ") = " << factorial(factorialVal) << '\n';
    cout << "isPrime(" << primeVal << ") = " << (isPrime(primeVal) ? "true" : "false") << '\n';
    cout << "gcd(" << gcdA << ", " << gcdB << ") = " << gcd(gcdA, gcdB) << '\n';
    cout << "fibonacci(" << fibonacciVal << ") = " << fibonacci(fibonacciVal) << '\n';
}

int main() {
    int choice;
    do {
        showMenu();
        cin >> choice;

        try {
            switch (choice) {
                case 1: BasicOperations(); break;
                case 2: MathFunctions(); break;
                case 3: Array(); break;
                case 4: Integers(); break;
                case 5: demoMode(); break;
                case 0: break;
                default: cout << "Invalid option!\n";
            }
        } catch (const exception &e) {
            cerr << e.what() << '\n';
        }

        cout << endl;

    } while (choice != 0);
}
