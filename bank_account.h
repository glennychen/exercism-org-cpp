#pragma once
#include <mutex>
#include <atomic>

namespace Bankaccount {

    class Bankaccount{

    private:
        std::atomic<bool> is_open=false;
        std::mutex mtx;
        long long curr_balance=0;
        static inline std::atomic<int> id_generator{1000};
        long long account_id;

    public:
        Bankaccount(): account_id{id_generator++} {}
        Bankaccount(const Bankaccount&) = delete;
        Bankaccount& operator=(Bankaccount&) = delete;
        Bankaccount(Bankaccount&&) = delete;
        Bankaccount& operator=(Bankaccount&&) = delete;
        ~Bankaccount() = default;

        long long balance();
        void deposit(long long money);
        void withdraw(long long money);
        void open();
        void close();
    };    
}
