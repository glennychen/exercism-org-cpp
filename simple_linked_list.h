#pragma once
#include<cstddef> //C standard definitions for std::size_t
#include<stdexcept>
namespace linked_list {
    template<typename T>
    struct Node{
        T val;
        Node* next;
        Node(T v): val{v}, next{nullptr} {}
    };

    template<typename T>
    class List{
        Node<T> *head=nullptr;
        std::size_t cnt;
    public:
        void push(T v);
        T pop();
        void unshift(T v);
        T shift();
        int count();
        void erase(T v);
    };

}  // namespace linked_list
