#include <iostream>     // std::cout
#include <algorithm>    // std::make_heap, std::pop_heap, std::push_heap, std::sort_heap
#include <vector>       // std::vector

namespace {
struct Node {
  Node(const int value) : value_(value) {}
  int value_;
};

class NodeCompare {
public:
  bool operator() (const Node* lhs, const Node* rhs) {
    return lhs->value_ > rhs->value_;
  }
};

void PrintNodes(const std::vector<Node*> nodes) {
  for (const auto& node: nodes) {
    std::cout<<node->value_<<" ";
  }
  std::cout<<std::endl;
}

}  // namespace 
int main () {
  if (false) {
    int myints[] = {10,20,30,5,15};
    std::vector<int> v(myints,myints+5);
  
    std::make_heap (v.begin(),v.end());
    std::cout << "initial max heap   : " << v.front() << '\n';
  
    std::pop_heap (v.begin(),v.end()); v.pop_back();
    std::cout << "max heap after pop : " << v.front() << '\n';
  
    v.push_back(99); std::push_heap (v.begin(),v.end());
    std::cout << "max heap after push: " << v.front() << '\n';
  
    std::sort_heap (v.begin(),v.end());
  
    std::cout << "final sorted range :";
    for (unsigned i=0; i<v.size(); i++)
      std::cout << ' ' << v[i];
  
    std::cout << '\n';
  } else {
    std::vector<Node*> heap_nodes_ = {
      new Node(100),
      new Node(20),
      new Node(10),
      new Node(50),
      new Node(100),
      new Node(1e6),
      new Node(1e6),
      new Node(1e6),
      new Node(1e6),
      new Node(1e6),
    };
    PrintNodes(heap_nodes_);
    std::cout<<"std::make_heap"<<std::endl;
    std::make_heap(heap_nodes_.begin(), heap_nodes_.end(), NodeCompare());
    PrintNodes(heap_nodes_);
    std::cout<<"change value in location 1"<<std::endl;
    heap_nodes_[1]->value_ = -21;
    heap_nodes_[8]->value_ = -2;
    PrintNodes(heap_nodes_);
    std::cout<<"std::pop_heap"<<std::endl;
    std::pop_heap(heap_nodes_.begin(), heap_nodes_.end(), NodeCompare());
    heap_nodes_.pop_back();
    PrintNodes(heap_nodes_);
    std::cout<<"std::pop_heap"<<std::endl;
    std::pop_heap(heap_nodes_.begin(), heap_nodes_.end(), NodeCompare());
    heap_nodes_.pop_back();
    PrintNodes(heap_nodes_);
    std::cout<<"std::pop_heap"<<std::endl;
    std::pop_heap(heap_nodes_.begin(), heap_nodes_.end(), NodeCompare());
    heap_nodes_.pop_back();
    PrintNodes(heap_nodes_);
    std::cout<<"std::pop_heap"<<std::endl;
    std::pop_heap(heap_nodes_.begin(), heap_nodes_.end(), NodeCompare());
    heap_nodes_.pop_back();
    PrintNodes(heap_nodes_);
  }
  return 0;
}
