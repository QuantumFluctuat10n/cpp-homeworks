#include "MathLib.h"
#include <stdexcept>

double add(double a, double b){
    return a + b;
}

double subtract(double a, double b){
    return a - b;
}

double multiply(double a, double b){
    return a * b;
}

double divide(double a, double b){
    if(b == 0){
        throw std::invalid_argument("Division on zero");
    }
    return a / b;
}
//------------------------------------------
double power(double base, int exponent){
    if(exponent < 0) return power(1/base, -exponent);
    if(exponent == 0) return 1;

    if(exponent%2 == 0) 
        return power(base*base, exponent/2);
    else
        return base*power(base*base, (exponent-1)/2);
}

double squareRoot(double value){
    if(value < 0){
        throw std::invalid_argument("Negative value");
    }
    if(value == 0) return 0;

    double temp;
    double root = value / 2;
    do {
        temp = root;
        root = (temp + (value / temp)) / 2;
    } while ((temp - root) != 0);
    return root;
}

double absoluteValue(double value){
    if(value < 0) return -value;
    else return value;
}
//---------------------------------------------
int factorial(int n){
    if(n < 0){
        throw std::invalid_argument("Negative value");
    }

    if(n == 0 || n == 1) return 1;
    int result = 1;
    for(int i = 2; i <= n; ++i){
        result *= i;
    }
    return result;
}

bool isPrime(int number){
    if (number <= 1) return false;
    for (int i = 2; i * i <= number; ++i) {
        if (number % i == 0) return false;
    }
    return true;
}

int gcd(int a, int b){
    if (b == 0) return a;
    return gcd(b, a % b);
}
//-----------------------------------------------------
double findMax(const double arr[], int size){
    if(size <= 0) throw std::invalid_argument("Negative size");
    double max = arr[0];
    for(int i = 1; i < size; ++i){
        if(arr[i] > max) max = arr[i];
    }
    return max;
}

double findMin(const double arr[], int size){
    if(size <= 0) throw std::invalid_argument("Negative size");
    double min = arr[0];
    for(int i = 1; i < size; ++i){
        if(arr[i] < min) min = arr[i];
    }
    return min;
}

double calculateSum(const double arr[], int size){
    if(size <= 0) throw std::invalid_argument("Negative size");
    double sum = 0;
    for(int i = 0; i < size; ++i){
        sum += arr[i];
    }
    return sum;
}

double calculateAverage(const double arr[], int size){
    if(size <= 0) throw std::invalid_argument("Negative size");
    return calculateSum(arr, size) / size;
}

void sortArray(double arr[], int size){
    if(size <= 0) throw std::invalid_argument("Negative size");
    for(int i = 0; i < size - 1; ++i){
        for(int j = 0; j < size - i - 1; ++j){
            if(arr[j] > arr[j + 1]){
                double temp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = temp;
            }
        }
    }
}
//----------------------------------------------------
bool isEven(int number){
    if(!number) return false;
    return number % 2 == 0;
}

bool isOdd(int number){
    if(!number) return true;
    return number % 2 != 0;
}

int fibonacci(int n){
    if(n < 0){
        throw std::invalid_argument("Negative value");
    }
    if(n == 0) return 0;
    if(n == 1) return 1;

    int a = 0, b = 1;
    for(int i = 2; i <= n; ++i){
        int next = a + b;
        a = b;
        b = next;
    }
    return b;
}