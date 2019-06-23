#include <iostream>
#include <vector>
#include <deque>

namespace {
enum Color {
	BLACK = 0,
	GRAY,
	WHITE,
};

struct Node {
	Node() : color(Color::BLACK), bfs_prev(nullptr), bfs_distance(0) {}
	Node(const std::string& name) : name(name), 
	  color(Color::BLACK), bfs_prev(nullptr), bfs_distance(0) {}

	std::string name;
	Color color;
	std::vector<Node*> neighbors;
	Node* bfs_prev;
	int bfs_distance;
	Node* dfs_prev;
	int dfs_find_time;
	int dfs_finish_time;
};

void AddNeighbors(Node* vertex, std::vector<Node*> neighbors) {
	for (const auto& neighbor : neighbors) {
		vertex->neighbors.push_back(neighbor);
	}
}

Node* BFS(Node* start, Node* final) {
	if (start == nullptr) {
		return nullptr;
	}
	if (final == nullptr) {
		return nullptr;
	}
	if (start == final) {
		std::cout <<start->name<<" == "<<final->name<<std::endl;
		return start;
	}
	std::deque<Node*> q;
	q.push_back(start);
	while (!q.empty()) {
		Node* current_node = q.front();
		q.pop_front();
		if (current_node == final) {
			return current_node;
		}
		if (current_node->color == Color::WHITE) {
			continue;
		}
		current_node->color = Color::GRAY;
		std::cout<<"processing "<<current_node->name<<std::endl;
		std::cout<<"visiting neighbors ";
		for (const auto& neighbor : current_node->neighbors) {
			if (neighbor->color != Color::BLACK) {
				continue;
			}
			std::cout<<neighbor->name<<" ";
			neighbor->bfs_prev = current_node;
			neighbor->bfs_distance = current_node->bfs_distance + 1;
			q.push_back(neighbor);
		}
		std::cout<<std::endl;
		current_node->color = Color::WHITE;
	}
	return nullptr;
} 


void BFSResetVertices(std::vector<Node*>& vertices) {
	for (auto& vertex: vertices) {
		vertex->color = Color::BLACK;
		vertex->bfs_prev = nullptr;
		vertex->bfs_distance = 0;
	}
}
void PrintPredeccsorTree(Node* node) {
	if (node->bfs_prev != nullptr) {
		PrintPredeccsorTree(node->bfs_prev);
	}
	std::cout<<node->name<<" -> ";
}
void BFSPath(Node* a, Node* b, std::vector<Node*>& vertices) {
	BFSResetVertices(vertices);
	std::cout<<"Trying to find path from "<<a->name<<" to "<<b->name<<std::endl;
	Node* found = BFS(a, b);
	if (found == nullptr) {
		std::cout<<"No path from "<<a->name<<" to "<<b->name<<std::endl;
		return;
	}
	std::cout<<"Found path from ";
	PrintPredeccsorTree(found->bfs_prev);
	std::cout<<b->name<<" of length "<<b->bfs_distance<<std::endl;
}

class DFS {
private:
  void DFSResetVertices(std::vector<Node*>& vertices) {
	time = 0;
	for (auto& vertex: vertices) {
		vertex->color = Color::BLACK;
		vertex->dfs_prev = nullptr;
		vertex->dfs_finish_time = 0;
		vertex->dfs_find_time = 0;
	}
  }

  void DFSVisit(Node* node) {
  	if (node == nullptr) {
  		return;
  	}
	time++;
	node->dfs_find_time = time;
	node->color = Color::GRAY;
	for (auto& neighbor: node->neighbors) {
	  if (neighbor->color != Color::BLACK) {
		continue;
	  }
	  neighbor->dfs_prev = node;
	  DFSVisit(neighbor);
	}
	node->color = Color::WHITE;
	node->dfs_finish_time = time;
	time++;
  }

  int time;
public:

  void Process(std::vector<Node*>& vertices) {
  	DFSResetVertices(vertices);
  	for (auto& vertex: vertices) {
  	  if (vertex->color == Color::BLACK) {
  		DFSVisit(vertex);
  	  }
  	}
  }
  void Display(const std::vector<Node*>& vertices) {
  	for (const auto& vertex: vertices) {
  		std::cout<<vertex->name<<" found at "
  		  <<vertex->dfs_find_time<<" and finished processing at "
  		  <<vertex->dfs_finish_time<<std::endl;
  	}
  }

  void TopologicalDisplay(std::vector<Node*>& vertices) {
  	std::sort(vertices.begin(), vertices.end(), 
  		[](Node* a, Node* b) { 
  			if (a->dfs_finish_time != b->dfs_finish_time){
  			  return a->dfs_finish_time > b->dfs_finish_time; 
	  		} else {
			  return a->dfs_find_time > b->dfs_find_time; 	
	  		}
  		});
  	Display(vertices);	
  }

};

}  // namespace

int main(int argc, char** argv) {
	Node* a = new Node("a");
	Node* b = new Node("b");
	Node* c = new Node("c");
	Node* d = new Node("d");
	Node* e = new Node("e");
	Node* f = new Node("f");
	Node* g = new Node("g");
	Node* h = new Node("h");
	Node* i = new Node("i");
	Node* j = new Node("j");
	Node* k = new Node("k");
	Node* l = new Node("l");
	std::vector<Node*> vertices = 
	  {a, b, c, d, e, f, g, h, i, j, k, l};
	AddNeighbors(a, {b, c});
	AddNeighbors(b, {c, d, e});
	AddNeighbors(d, {h, i});
	AddNeighbors(h, {k});
	AddNeighbors(f, {e, a, c});
	AddNeighbors(i, {g, k});
	AddNeighbors(k, {a, b, c});
	BFSPath(a, l, vertices);
	BFSPath(a, k, vertices);
	BFSPath(k, a, vertices);

	DFS dfs;
	dfs.Process(vertices);
	//dfs.Display(vertices);
	dfs.TopologicalDisplay(vertices);
	return 0;
}