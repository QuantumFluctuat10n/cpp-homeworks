#include <iostream>
#include <thread>
#include <vector>
#include <random>

class UnsafeBankAccount
{
private:
    double balance = 1000.0;

public:
    void deposit(double amount)
    {
        balance += amount;
    }

    bool withdraw(double amount)
    {
        if (balance >= amount)
        {
            balance -= amount;
            return true;
        }
        return false;
    }

    double get_balance() const
    {
        return balance;
    }
};

class UnsafeBankStatistics
{
private:
    int transaction_count = 0;
    double total_amount = 0.0;

public:
    void record_transaction(double amount)
    {
        transaction_count++;
        total_amount += amount;
    }

    int get_total_transactions() const { return transaction_count; }
    double get_total_amount() const { return total_amount; }
};

void cashierWork(UnsafeBankAccount &bankAccount, UnsafeBankStatistics &statist)
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> deposit(50, 500);
    std::uniform_int_distribution<> withdraw(10, 200);
    std::bernoulli_distribution operation(0.5);

    for (int i = 0; i < 100; i++)
    {
        if (operation(gen))
        {
            double amount = deposit(gen);
            bankAccount.deposit(amount);
            statist.record_transaction(amount);
        }
        else
        {
            double amount = withdraw(gen);
            if (bankAccount.withdraw(amount))
                statist.record_transaction(amount);
        }
    }
}

int main()
{
    UnsafeBankAccount unsafeAccount;
    UnsafeBankStatistics statist;
    std::vector<std::thread> threads(5);
    for (int i = 0; i < 5; i++)
    {
        threads.emplace_back(cashierWork, std::ref(unsafeAccount), std::ref(statist));
    }

    for (auto &thread : threads)
    {
        if (thread.joinable())
        {
            thread.join();
        }
    }

    std::cout << "================RESULT================\n";
    std::cout << "Total transactions: " << statist.get_total_transactions() << std::endl;
    std::cout << "Total transactions amount: " << statist.get_total_amount() << std::endl;
    std::cout << "Final balance: " << unsafeAccount.get_balance() << std::endl;
}