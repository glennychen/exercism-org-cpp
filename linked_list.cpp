#include "linked_list.h"

namespace linked_list {
        template<typename T>
        void List<T>::push(T v){
            if(!head){
                Node<T>* n=new Node<T>(v);
                head=n;
                n->next=nullptr;
                ++cnt;
            } else {
                Node<T>* n=head;
                while(n->next){
                    n=n->next;
                }
                n->next=new Node<T>(v);
                ++cnt;
            }
        }

        template<typename T>
        T List<T>::pop(){
            if(!head){
                throw std::runtime_error("cannot pop empty list");
            }
            
            Node<T>* curr=head;
            Node<T>* prev=nullptr;
            while(curr->next){
                prev=curr; 
                curr=curr->next;                   
            }

            if(prev){
                //the while loop run at least once
                prev->next=curr->next;
            } else {
                //there is only one node, remember to set head=nullptr or head will become a dangling ptr
                head=nullptr;
            }
            T res=std::move(curr->val);
            delete curr;            
            --cnt;
            return  res;
        }

        template<typename T>
        void List<T>::unshift(T v){
            Node<T>* node=new Node<T>(v);
            node->next=head;
            head=node;
            ++cnt;
        }

        template<typename T>
        T List<T>::shift(){
            if(!head){
                throw std::runtime_error("cannot shift empty list");
            }
            //Node<T> *dummy= &Node<T>(-1); //likely not what people want, why assign a temporary?
            Node<T> dd(-1);
            Node<T> *dummy=&dd;
            dummy->next=head->next;
            int res=head->val;
            delete head;
            --cnt;
            head=dummy->next;
            return res;
        }

        template<typename T>
        int List<T>::count(){
            // Node<T>* n=head;
            // while(n){
            //     n=n->next;
            //     ++cnt;
            // }
            return static_cast<int>(cnt);
        }

        template<typename T>
        void List<T>::erase(T v){
            Node<T> dd(-1);
            Node<T>* dummy=&dd;
            dummy->next=head;
            Node<T>* curr=head;
            while(curr){
                if(curr->val==v){
                    dummy->next=curr->next;
                    delete curr;
                    --cnt;
                    return;
                } else {
                    dummy=curr;
                    curr=curr->next;
                }
            }
            return;
        }

    template class List<int>; //
}  // namespace linked_list
