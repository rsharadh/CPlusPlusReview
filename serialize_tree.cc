#include <iostream>
#include <memory>
#include <deque>
#include <vector>
#include <stack>   
#include <string>

namespace {
template <class T>	
struct Node {
	Node() : value(0), left(nullptr), right(nullptr) {}
	Node(const T value) : value(value), left(nullptr), right(nullptr) {}
	Node(const T value, Node* left, Node* right) : value(value), left(left), right(right) {}
	void AddLeft(Node<T>* left) {
		this->left = left;
	}
	void AddRight(Node<T>* right) {
		this->right = right;
	}
	
	T value;
	Node* left;
	Node* right;
};

typedef Node<std::string> NodeS;

void LevelOrderTraversal(NodeS* root) {
	std::deque<NodeS*> current_queue;
	std::deque<NodeS*> next_queue;
	current_queue.push_back(root);
	while (!current_queue.empty() || !next_queue.empty()) {
		while (!current_queue.empty()) {
			auto current_node = current_queue.front();
			std::cout<<current_node->value<<" ";
			if (current_node->left != nullptr) {
				next_queue.push_back(current_node->left);
			}
			if (current_node->right != nullptr) {
				next_queue.push_back(current_node->right);
			}
			current_queue.pop_front();
		}
		std::cout<<std::endl;
		current_queue = std::move(next_queue);
		next_queue.clear();
	}
}

void InorderTraverse(NodeS* root) {
	if (root == nullptr) {
		return;
	}
	InorderTraverse(root->left);
	std::cout<<" "<<root->value;
	InorderTraverse(root->right);

}

void PostorderTraverse(NodeS* root) {
	if (root == nullptr) {
		return;
	}
	PostorderTraverse(root->left);
	PostorderTraverse(root->right);
	std::cout<<" "<<root->value;
}


char UNIQUE_MARKER = '&';

std::string CheapSerializeTree(NodeS* root) {
	if (root == nullptr) {
		return std::string(1, UNIQUE_MARKER);
	}
	const auto& left_serialized = CheapSerializeTree(root->left);
	const auto& right_serialized = CheapSerializeTree(root->right);
	return 
		  root->value + ","
		  + left_serialized + ","
		  + right_serialized;
}


NodeS* CheapDeSerializeTree(const char* serialized, int& start, int end) {
	if (serialized[start] == UNIQUE_MARKER) {
		start += 2;
		return nullptr;
	}
	int index = start;
	for (;index < end; index++) {
		if (serialized[index] == ',') {
			break;
		}
		//std::cout<<serialized[index]<<" ";
	}
	//std::cout<<std::endl;
	NodeS* node = new NodeS(
		std::string(&serialized[start], &serialized[index]));
	index += 1;
	start = index;
	//std::cout<<"node->value "<<node->value<<std::endl;
	node->left = CheapDeSerializeTree(serialized, start, end);
	node->right = CheapDeSerializeTree(serialized, start, end);
	return node;
}

std::string ExpensiveSerializeTree(NodeS* root) {
	if (root == nullptr) {
		return "()";
	}
	const auto& left_serialized = ExpensiveSerializeTree(root->left);
	const auto& right_serialized = ExpensiveSerializeTree(root->right);
	return 
		 "(" +root->value 
		  + left_serialized 
		  + right_serialized + ")";
}

NodeS* ExpensiveDeSerializeTree(
	const char* serialized, int start, int end) {
	if (end - start + 1 < 7) {
		return nullptr;
	}

	if (serialized[start] != '(' || serialized[end] != ')') {
		throw std::string("input string not in expected format.");
	}

	NodeS* return_ptr = nullptr;
	if (serialized[start + 1] != '(' 
		&& serialized[start + 1] != ')') {
		return_ptr = new NodeS(std::string(1, serialized[start + 1]));
		// Identify left-right split.
		if (serialized[start + 2] != '(') {
			throw std::string("input string not in expected format.");
		}
		int split = start + 3;
		int num_unbalanced_left_parenthesis = 1;
		for (; split != end - 1;++split) {
			switch (serialized[split]) {
				case '(' :
					num_unbalanced_left_parenthesis++;
					break;
				case ')' :
					num_unbalanced_left_parenthesis--;
					break;
				default:
					break;	
			}
			if (num_unbalanced_left_parenthesis == 0) {
				break;
			}
		}
		// generate left child, start + 2 to split
		return_ptr->left = ExpensiveDeSerializeTree(
			serialized, start + 2, split);
		// generate right child, split + 1 to end -1
		return_ptr->right = ExpensiveDeSerializeTree(
			serialized, split + 1, end - 1);
	} else {
		throw std::string("input string not in expected format.");
	}
	return return_ptr;
}



}  // namespace

int main(int argc, char** argv) {
	auto a = new NodeS("A");
	auto b = new NodeS("B");
	auto c = new NodeS("C");
	auto d = new NodeS("D");
	auto e = new NodeS("E");
	auto f = new NodeS("F");
	a->AddLeft(b);
	a->AddRight(c);
	b->AddLeft(d);
	b->AddRight(e);
	e->AddRight(f);


	std::cout << "Original tree \n LevelOrderTraversal "<<std::endl;
	LevelOrderTraversal(a);

	std::cout << "InorderTraverse ";
	InorderTraverse(a);
	std::cout<<std::endl;
	std::cout << "PostorderTraverse ";
	PostorderTraverse(a);
	std::cout<<std::endl;


	std::cout << "Expensive Serialize and DeSerialize "<<std::endl;
	const auto& serialized_tree = ExpensiveSerializeTree(a);
	std::cout<<serialized_tree<<std::endl;
	try {
		NodeS* root = ExpensiveDeSerializeTree(serialized_tree.c_str(), 0, serialized_tree.size() - 1);
		LevelOrderTraversal(root);
	  
	  	std::cout << "InorderTraverse ";
	  	InorderTraverse(root);
	  	std::cout<<std::endl;
	    std::cout << "PostorderTraverse ";
	  	PostorderTraverse(root);
	  	std::cout<<std::endl;
	} catch (std::string& e) {
		std::cout<<"exception thrown "<<e<<std::endl;
	}

	std::cout << "Cheap Serialize and DeSerialize"<<std::endl;
		const auto& cheap_serialized_tree = CheapSerializeTree(a);
	std::cout<<cheap_serialized_tree<<std::endl;
	int start = 0;
	NodeS* root = CheapDeSerializeTree(
		cheap_serialized_tree.c_str(), 
		start, 
		cheap_serialized_tree.size() - 1); 	
		std::cout << "InorderTraverse ";
		InorderTraverse(root);	
		std::cout<<std::endl;
	std::cout << "PostorderTraverse ";
		PostorderTraverse(root);
		std::cout<<std::endl;

  return 0;
}