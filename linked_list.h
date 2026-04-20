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

        T pop_shift_helper(Node<T>* target){
            T res=std::move(target->val);
            if(head==tail && head!=nullptr){
                //only one node
                head=tail=nullptr;
            }
            if(target==tail){
                tail=tail->prev;
                tail->next=nullptr;
            } else if(target==head){
                head=head->next;
                head->prev=nullptr;
            }
           
            delete target;
            --cnt;
            return res;
        }
    
        T pop(){
            if(!tail) {throw std::runtime_error("cannot pop empty list");}
            return pop_shift_helper(tail);         
        }
    
        T shift(){
            if(!head) {throw std::runtime_error("cannot shift empty list");}
            return pop_shift_helper(head);         
        }

        void push_unshift_helper(T v, Node<T>* next=nullptr, Node<T>* prev=nullptr){
            Node<T>* new_node=new Node<T>(v, next, prev);
            if(prev){
                tail->next=new_node;
                tail=new_node;
            } else if(next){
                head->prev=new_node;
                head=new_node;
            } else {
                head=tail=new_node;
            }
            ++cnt;
        }
        void push(T v){
            push_unshift_helper(v, nullptr, tail);
        }
    
        void unshift(T v){           
            push_unshift_helper(v, head, nullptr);
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
