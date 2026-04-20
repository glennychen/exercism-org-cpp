#pragma once
#include<cstddef> // C standard definitions for std::size_t
#include<stdexcept>

namespace linked_list {
    template<typename T>
    struct Node{
        T val;
        Node* next;
        Node* prev;
        Node(T v, Node* next=nullptr, Node* prev=nullptr): val{v}, next{next}, prev{prev} {}
    };

    template<typename T>
    class List{
        Node<T> *head=nullptr;
        Node<T> *tail=nullptr;
        int cnt{0};

    public:
        List() = default; //explicity
        List(const List& other): head{nullptr}, tail{nullptr}, cnt{0} {
            Node<T> *curr= other.head;
            while(curr){
                push(curr->val); //DRY principle: reuse our push() to implmement copy constructor
                curr=curr->next;
            }
        }
        List& operator=(const List& rhs){           
            if(this==&rhs) return *this; //compare the actual memory location

            // use copy constructor
            List<T> copy(rhs); 

            // move the new copy in, and move the old data into the copy
            // so the local "copy" will self-destruct with the old data
            std::swap(head,copy.head);
            std::swap(tail,copy.tail);
            std::swap(cnt,copy.cnt);
            
            //but I might already allocate a lot of Nodes, what is the strategy to reuse them?
            //in particualr, what if the count of rhs is not equal to lhs? messy implmentation incoming...
            
            return *this;            
        }
        List(List&& other): head{other.head}, tail{other.tail}, cnt{other.cnt}{
            // after moving, maintain other in a a valid state, rhs has null head, null tail, and 0 cnt
            other.head=nullptr;
            other.tail=nullptr;
            other.cnt=0;
        }
            
        List& operator=(List&& rhs){ //cannot use const in the arg, we need to move it
            if(this==&rhs) return *this;
            
            std::swap(head,rhs.head);
            std::swap(tail,rhs.tail);
            std::swap(cnt,rhs.cnt);

            // is it better to clean up rhs for deterministic reason? or let rhs die when it is out-of-scope?
            rhs.clear(); 
            
            return *this;
        }
        ~List(){
            clear(); //because std::vector also has one
            head=tail=nullptr;
            cnt=0;
        }
        void clear(){
            while(head){
                    Node<T>* old_head=head;
                    head=head->next;
                    delete old_head;
            }
            head=tail=nullptr;
            cnt=0;
         }

        void push(T v){
            Node<T>* new_tail=new Node<T>(v, nullptr, tail);
            if(tail){
                tail->next=new_tail;
                tail=new_tail;
            } else {
                head=tail=new_tail;
            }
            ++cnt;
        }
    
        T pop(){
            if(!tail) { throw std::runtime_error("cannot pop empty list"); }
            T res=std::move(tail->val);
            Node<T>* old_tail=tail;
            tail=tail->prev;
            if(tail){
                tail->next=nullptr;
            } else{
                head=tail=nullptr;               
            }
            delete old_tail;
            --cnt;
            return res;
        }
    
        T shift(){
            if(!head) {throw std::runtime_error("cannot shift empty list");}
            Node<T>* old_head=head;
            T res=std::move(head->val);
            head=head->next;
            if(head){
                head->prev=nullptr;
            } else {
                head=tail=nullptr;
            }
            delete old_head;
            --cnt;           
            return res; //copy elision? Jason said which c++ standardd is guaranteed? or just return by value almost always like usual?
        }
    
        void unshift(T v){
            Node<T>* new_head=new Node<T>(v, head, nullptr);
            if(head){
                head->prev=new_head;
                head=new_head;
            } else {
                head=tail=new_head;
            }            
            ++cnt;
        }
        
        int count(){
            return cnt;
        }
    
        void erase(T v){
            // dummy node not that useful as in single-linked list, we already have the "prev"
            // Node<T> dummy{-1,head,nullptr};
            // Node<T>* d=&dummy;
            Node<T>* curr=head;
            while(curr){
                if(curr->val==v){
                    if(curr->prev){
                        curr->prev->next=curr->next;
                    } else {
                        head=curr->next;
                        if(head) head->prev=nullptr; //isn't this necessary?house keeping new head->prev
                    }
                    
                    if(curr->next) {
                        curr->next->prev=curr->prev;
                    } else {
                        tail=curr->prev;
                        if(tail) tail->next=nullptr; //isn't this necessary?house keeping new tail->next
                    }
                    
                    delete curr;
                    --cnt;
                    return;
                }
                curr=curr->next;
            }
        }
    };

}  // namespace linked_list
