#include "bank_account.h"

namespace Bankaccount {
        long long Bankaccount::balance(){
            std::lock_guard<std::mutex> lk(mtx);
            if(is_open==false) throw std::runtime_error("cannot check balance on close account");
            return curr_balance;
        }
        void Bankaccount::deposit(long long money){
            std::lock_guard<std::mutex> lk(mtx);
            if(is_open==false) throw std::runtime_error("cannot deposit on close account");
            if(money<0){
                throw std::runtime_error("cannot deposit negative");
            }
            curr_balance+=money;
        }
        void Bankaccount::withdraw(long long money){
            std::lock_guard<std::mutex> lk(mtx);
            if(is_open==false) throw std::runtime_error("cannot withdraw on close account");
            if(curr_balance<money){
                throw std::runtime_error("cannot withdraw more than deposit");
            }
            if(money<0){
                throw std::runtime_error("cannot withdraw negative");
            }
            curr_balance-=money;
        }
        void Bankaccount::open(){
            std::lock_guard<std::mutex> lk(mtx);
            if(is_open==false){
                 is_open=true;
                 curr_balance=0;
            } else {
                throw std::runtime_error("Cannot open an already opened account");
            }                    
        }  
        void Bankaccount::close(){
            std::lock_guard<std::mutex> lk(mtx);
            if(is_open==false) throw std::runtime_error("cannot close already closed");      
            is_open=false;
        }    
  
}
