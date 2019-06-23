#include <iostream>
#include <exception>
#include <memory>
#include <string>
// struct N {
//     N* n;
//     int v;
// };

// N* h = nullptr;

// void foo(int x) {
//     //N* n = new N{h, x};
//     N* n = new N();
//     n->n = h;
//     n->v = x;
//     h = n;
// }


// // print the values of v in the list, starting at node n.
// void print_list(N* n) {
//     // please enter your implementation here
//   if (n == nullptr) {
//     std::cerr<<"Nullptr input to print_list"<<std::endl;
//     return;
//   }
//   while (n != nullptr) {
//     std::cout<<n->v<<" ";
//     n = n->n;
//   }
//   std::cout<<std::endl;
//  }


// // print the values of v in the list, starting at node n, in reverse
// void print_list_reverse(N* n) {
//     // please enter your implementation here
//   if (n == nullptr) {
//     return;
//   }
//   print_list_reverse(n->n);
//   std::cout<<n->v<<" ";
// }

// Hey David, I lost you. Could not hear anything that you spoke about after I mentioend the need for copy/destructor/move constructors and copy assignment/move assignment operators?
//
// please implement a class with the following methods:
// void push_front(int);
// int pop_back();

template<class T>
struct N {
    N* n;
    T v;
};

template<class T>
class Queue {
  N<T>* front;
  N<T>* back;
  public:
  Queue() : front(nullptr), back(nullptr) {}
  
  void push_front(T value) {
    N<T>* n = new N<T>();
    n->v = std::move(value);
    n->n = nullptr;
    if (front != nullptr) {
      front->n = n;
    }
    front = n;

    if (back == nullptr) {
      back = n;
    }
    
  }
  
  T pop_back() {
    if (back == nullptr) {
      throw std::runtime_error("not allowed");
    }
    T v = back->v;
    N<T>* n = back;
    if (front == back) {
      front = nullptr;
      back = nullptr;
    } else {
      back = back->n;
    }
    
    delete n;
    return v;
  }
  
};

int main(void) {
    // foo(3);
    // foo(2);
    // foo(1);
    //print_list(h);
    //print_list_reverse(h);
    // Queue<int> q;
    // q.push_front(1);
    // q.push_front(2);
    // q.push_front(3);
    // std::cout<<q.pop_back()<<" "<<q.pop_back()<<" "<<q.pop_back()<<std::endl;
  
    Queue<std::string> q_s;
  q_s.push_front("hello");
  q_s.push_front("world");
  q_s.push_front("!");
  std::cout<<q_s.pop_back()<<" "<<q_s.pop_back()<<" "<<q_s.pop_back()<<" "<<std::endl;
  
    Queue<std::unique_ptr<std::string>> q_u;
    q_u.push_front(std::move(std::make_unique<std::string>("abc")));
    return 0;
}
