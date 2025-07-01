#include <iostream>
#include <functional>
#include <vector>
#include "BigData"

void bigData(){
    BigData a(5);
    std::cout << "a: ";
    a.print();

    std::cout << "\nc = move(a):\n";
    BigData c(0);
    c = std::move(a);

    std::cout << "c: ";
    c.print();
    std::cout << "\na: ";
    a.print();
}

void lambda(){
    std::vector<int> v = { 1, 2, 3, 4, 5 };

    std::cout << "before: ";
    for(const auto& n : v)
        std::cout << n << ' ';
    
    std::function<void(int)> func = [&v](int add){
        for(auto& n : v)
            n+=add;
    };
    func(5);

    std::cout << "\nafter: ";
    for(const auto& n : v)
        std::cout << n << ' ';
}

int main(){
    std::cout << "Move semantics:\n";
    bigData();
    std::cout << "\nLambda function:\n";
    lambda();
}