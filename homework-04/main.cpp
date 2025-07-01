#include <iostream>
#include <thread>
#include <vector>

#include "OrderSystem"

void OrdersProcessing(){
    OrderProcessor Processor;
    std::vector<Order> rawOrders = {
        Order(1, {"item1", "item2"}),
        Order(2, {"item1"}),
        Order(3, {})
    }, validOrders, pricedOrders, finalOrders;

    std::thread orderValidator([&](){
        validOrders = Processor.validateOrders(rawOrders);
    });
    orderValidator.join();

    std::thread pricingCalculator([&](){
        pricedOrders = Processor.calculatePricing(validOrders);
    });
    pricingCalculator.join();

    std::thread inventoryChecker([&](){
        finalOrders = Processor.checkInventory(pricedOrders);
    });
    inventoryChecker.join();

    std::thread invoiceGenerator([&](){
        Processor.generateInvoices(finalOrders);
    });
    invoiceGenerator.join();
}

void factorial(int num){
    long long result = 1;
    for(int i = 1; i <= num; ++i){
        result *= i;
    }
    std::cout << "Factorial of " << num << " is " << result << std::endl;
}
void sumOfSquares(int num){
    long long sum = 0;
    for(int i = 1; i <= num; ++i){
        sum += i * i;
    }
    std::cout << "Sum of squares from 1 to " << num << " is " << sum << std::endl;
}
void fibonacci(int num){
    long long a = 0, b = 1;
    std::cout << "Fibonacci series up to " << num << ": ";
    for(int i = 0; i < num; ++i){
        std::cout << a << " ";
        long long next = a + b;
        a = b;
        b = next;
    }
    std::cout << std::endl;
}
void primeNums(int num){
    std::cout << "Prime numbers up to " << num << ": ";
    for(int i = 2; i <= num; ++i){
        bool isPrime = true;
        for(int j = 2; j * j <= i; ++j){
            if(i % j == 0){
                isPrime = false;
                break;
            }
        }
        if(isPrime)
            std::cout << i << " ";
    }
    std::cout << std::endl;
}

void FourThreads(){
    std::vector<std::thread> threads;
    threads.emplace_back(factorial, 15);
    threads.emplace_back(sumOfSquares, 1000);
    threads.emplace_back(fibonacci, 30);
    threads.emplace_back(primeNums, 100);

    for(auto& thread : threads){
        if(thread.joinable())
            thread.join();
    }

}

int main(){
    std::cout << "Calculating numbers with four threads:\n";
    FourThreads();
    std::cout << "We get something unreadable\n";

    std::cout << "\nOrder processing simulation:\n";
    OrdersProcessing();
}