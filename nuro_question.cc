// Input:
// A < B
// A < C
// B < D
// C < D

// Output: (any valid order)
// A, B, C, D
// A, C, B, D


#include <iostream>
#include <algorithm>
#include <utility> 
#include <string>
#include <vector>
#include <unordered_map>
using namespace std;

namespace {

enum Color {
  BLACK = 0,
  GRAY,
  WHITE
};

struct Node {
  Node() {
    children.clear();
  }
  string name;
  vector<Node*> children;

  // DFS properties
  Color color;
  int find_time;
  int finish_time;
  Node* prev;
};

class Prereq {
  public:
  Prereq() {
   name_to_node_map.clear();
   graph_vertices.clear();
   time = 0;
  }

  void Add(const vector<pair<string, string>>& input_pairs) {
    for (const auto& pair : input_pairs) {
      Add(pair);
    }
  }
  void Add(const pair<string, string>& input_pair) {
    const auto& req_class = input_pair.first;
    const auto& next_class = input_pair.second;

    Node* req_vertex = FindOrCreate(req_class);
    Node* next_vertex = FindOrCreate(next_class);
    AddChild(req_vertex, next_vertex);
  }

  Node* FindOrCreate(const string& name) {
    if (name_to_node_map.find(name) == name_to_node_map.end()) {
      Node* new_vertex = new Node();
      new_vertex->name = name;
      name_to_node_map[name] = new_vertex;
      graph_vertices.push_back(new_vertex);
    }
    return name_to_node_map[name];
  }

  void AddChild(Node* parent, Node* child) {
    // Skipping check if child is already a child of parent
    parent->children.push_back(child);
  }

  ~Prereq() {
    for (int index = 0; index < graph_vertices.size(); index++) {
      delete graph_vertices[index];
    }
  }

  void Display() {
    for (const auto& node_pointer: graph_vertices) {
      std::cout<<"Node name "<<node_pointer->name<<" with children ";
      for (const auto& child : node_pointer->children) {
        std::cout<<child->name<<" ";
      }
      std::cout<<std::endl;
    }
  }

  void TopologicalSortAndDisplay() {
    DFS();
    sort(graph_vertices.begin(), graph_vertices.end(),
    [](Node* a, Node* b) {
      return a->finish_time > b->finish_time;
    });

    std::cout<<"Topological sort \n";
    for (const auto& node_pointer: graph_vertices) {
      std::cout<<"Node name "<<node_pointer->name<<" with finish time "
      <<node_pointer->finish_time<<endl;
    }
  }

  void DFS() {
    for (auto node_pointer: graph_vertices) {
      if (node_pointer->color == BLACK) {
        DFSVisit(node_pointer);
      }
    }
  }

  private:

  void DFSVisit(Node* vertex) {
    if (vertex->color == WHITE) {
      return;
    }
    time++;

    vertex->find_time = time;
    for (auto child: vertex->children) {
      if (child->color == BLACK) {
        child->color=GRAY;
        child->prev = vertex;
        DFSVisit(child);
      }
    }
    time++;
    vertex->finish_time = time;
    vertex->color = WHITE;
  }

  int time;
  unordered_map<string, Node*> name_to_node_map;
  vector<Node*> graph_vertices;  
};


}  // namespace

/*
DFS complxity: O(V + E), every node is visited exactly once and every edge is visited exactly once.

Topological sort: O(VlogV + E), since we sort.

How long have  you been at nuro? what do you do at nuro?
*/
int main() {
	cout<<"Hello \n";

  vector<pair<string, string>> input = {
    {"A","B"}, {"A","C"}, {"B","D"}, {"C","D"}, {"D", "A"}};
  Prereq prereq_finder;
  prereq_finder.Add(input);
  prereq_finder.Display();

  prereq_finder.TopologicalSortAndDisplay();
	return 0;
}
