#pragma once
#include<cstddef> // C standard definitions for std::size_t
#include<stdexcept>
#include <limits.h>

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

    private:
        // invariant: That was one of the reasons I wanted to show the diagram. There are exactly two pointers pointing to each node.
        Node<T> *head; //head is always there!
        Node<T> *tail; //tail is always there!
        int cnt{0};

        void disaster_push_unshift_helper(T v, Node<T>* next=nullptr, Node<T>* prev=nullptr){
            Node<T>* new_node=new Node<T>(v, next, prev);
            if (!head) {
                head=tail=new_node;
            } else if(prev){
                tail->next=new_node;
                tail=new_node;
            } else if(next){
                head->prev=new_node;
                head=new_node;
            }
            ++cnt;
        }

        T disaster_pop_shift_helper(Node<T>* target){
            if(!tail || !head) {throw std::runtime_error("The list  is empty");}
            T res=std::move(target->val);
            if(head==tail && head!=nullptr){
                //only one node, after remove, everything is nullptr
                head=tail=nullptr;
            }else if(target==tail){
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

    public:
        List(){
            head = new Node<T>(T{});
            tail = new Node<T>(T{});
            head->next=tail;
            tail->prev=head;
            //note: cnt is not "two" because head and tail do not count
        }

        //copy constructor
        List(const List& other) {
            head=new Node<T>(T{});
            tail=new Node<T>(T{});
            head->next=tail;
            tail->prev=head;
            Node<T> *curr= other.head->next;

            //copy all the way right before other's tail
            while(curr!=other.tail){
                push(curr->val);
                curr=curr->next;
            }
        }

        //copy assignment
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
            //in particular, what if the list1.count != list2.count?
            return *this;
        }

        //move consturctor
        List(List&& other){
            //swap is a good idea in move assignment to swap both "valid" List
            //in move constructor, I find that swapping garbage to the rhs is quite dangerous, especially if the compiler is passed by optimization -O3. (compiler is free to re-use garbage! who knows what can happen. Struct no longer 64byes align, someting has garbage pointer, which is another UB ....using swap is not worth it....)
            head=other.head;
            tail=other.tail;
            cnt=other.cnt;

            //other must be in valid state, we need to give other the valid head and tail
            other.head=new Node<T>(T{});
            other.tail=new Node<T>(T{});
            other.head->next=other.tail;
            other.tail->prev=other.head;
        }

        //move assignment
        List& operator=(List&& rhs){ //cannot use const in the arg, we need to move it
            if(this==&rhs) return *this;
            std::swap(head,rhs.head);
            std::swap(tail,rhs.tail);
            std::swap(cnt,rhs.cnt);
            //let's not call rhs.clear()
            return *this;
        }
        ~List(){
            clear(); //because std::vector also has one
            //The only time we can delete the dummy head and tail value
            delete head;
            delete tail;
        }
        void clear(){
            if(head->next!=tail){
                Node<T>* curr=head->next;
                while(curr!=tail){
                    Node<T>* to_delete=curr;
                    curr=curr->next;
                    delete to_delete;
                }
            }

            //note: do not delete head and tail yet, this list MUST be in valid state
            //valid state include the case after the move constructor, 
            //the invariant is head and tail are permenant, if the List is valid 
            //A list with 0 elements are valid, for example
            head->next=tail;
            tail->prev=head;
            cnt=0; //just zero out this block of memory
         }
    
        void push(T v){
            insert_after(tail->prev,v);
        }

        void unshift(T v){
            insert_after(head,v); 
        }

        void insert_after(Node<T>* prev, T v){
            Node<T>* new_node=new Node<T>(v);
            new_node->prev=prev;
            new_node->next=prev->next;
            prev->next->prev=new_node;
            prev->next=new_node;
            ++cnt;
        }
    
        //BUGGY don't use. You can accidentally remove head. Use this->remove() instead
        // T remove_after(Node<T>* prev){
        //     if(head->next==tail) return -1; //list empty
        //     Node<T>* to_delete=prev->next            
        //     prev->next=to_delete->next;
        //     prev->next->prev=prev;

        //     T res=std::move(to_delete->val);
        //     --cnt;
            
        //     return res;
        // }
        //BUGGY don't use

        T pop(){
            return remove(tail->prev);            
        }

        T shift(){
           return remove(head->next);
        }  

        T remove(Node<T>* to_delete){
            if(head->next==tail) throw std::runtime_error("the list is empty");
            to_delete->prev->next=to_delete->next;
            to_delete->next->prev=to_delete->prev;

            T res=std::move(to_delete->val);
            delete to_delete;
            --cnt;
            
            return res;
        }
    
        void erase(T v){
            if(head->next==tail) throw std::runtime_error("the list is empty");
            Node<T>* curr=head->next;
            while(curr!=tail){
                if(curr->val==v){
                    remove(curr);
                    return;
                }
                curr=curr->next;
            }
        }
    


        int count() const{
            return cnt;
        }
    };

}  // namespace linked_list
