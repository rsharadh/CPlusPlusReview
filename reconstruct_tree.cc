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
		if (!next_queue.empty()) {
			std::cout<<std::endl;
		}
		current_queue = std::move(next_queue);
		next_queue.clear();
	}
	std::cout<<std::endl;
}

std::string InorderTraverse(NodeS* root) {
	if (root == nullptr) {
		return "";
	}
	std::string return_string;
	return_string += InorderTraverse(root->left);
	return_string += "";
	return_string += root->value;
	return_string += InorderTraverse(root->right);
	return return_string;
}

std::string PostorderTraverse(NodeS* root) {
	if (root == nullptr) {
		return "";
	}
	std::string return_string;
	return_string += PostorderTraverse(root->left);
	return_string += PostorderTraverse(root->right);
	return_string += "";
	return_string += root->value;
	return return_string;
}

std::string PreorderTraverse(NodeS* root) {
	if (root == nullptr) {
		return "";
	}
	std::string return_string;
	return_string += "";
	return_string += root->value;
	return_string += PreorderTraverse(root->left);
	return_string += PreorderTraverse(root->right);
	return return_string;
}


// Requires node values to be unique.
NodeS* Reconstruct(
	const char* inorder, const char* preorder, int length) {
	if (length == 0) {
		return nullptr;
	}
	// Find location of root in inorder traversal. 	
	int inorder_index = 0;
	for (; inorder_index < length; inorder_index++) {
		if (inorder[inorder_index] == preorder[0]) {
			break;
		}
	}
	NodeS* root = new NodeS(std::string(1, preorder[0]));


	// All nodes to the left of root in inorder traversal
	// form tree rooted at left child of root. Let the number
	// of such nodes be n. These n nodes are in the index range
	// 1 ... n - 1 in the pre-order traversal of tree.
	if (inorder_index <= length - 1) {
		root->left = Reconstruct(
				inorder, preorder + 1, inorder_index);
		root->right = Reconstruct(
			inorder + inorder_index + 1, 
			preorder + inorder_index + 1,
			length - inorder_index - 1);
	}
	return root;
}


}  // namespace

int main(int argc, char** argv) {
	auto a = new NodeS("A");
	auto b = new NodeS("B");
	auto c = new NodeS("C");
	auto d = new NodeS("D");
	auto e = new NodeS("E");
	auto f = new NodeS("F");
	auto g = new NodeS("G");
	auto h = new NodeS("H");
	a->AddLeft(b);
	a->AddRight(c);
	b->AddLeft(d);
	b->AddRight(e);
	e->AddRight(f);
	f->AddRight(g);
	g->AddLeft(h);


	std::cout << "Original tree \n LevelOrderTraversal "<<std::endl;
	LevelOrderTraversal(a);

	std::cout << "InorderTraverse ";
	std::cout<<InorderTraverse(a);
	std::cout<<std::endl;
	std::cout << "PostorderTraverse ";
	std::cout<<PostorderTraverse(a);
	std::cout<<std::endl;
	std::cout << "PreorderTraverse ";
	std::cout<<PreorderTraverse(a);
	std::cout<<std::endl;

	std::cout << "\n\n Reconstructing from inorder and preorder traversals "<<std::endl;
	auto inorder_traversed = InorderTraverse(a);
	auto preorder_traversed = PreorderTraverse(a);
	auto reconstructed_root = Reconstruct(
		inorder_traversed.c_str(), 
		preorder_traversed.c_str(), 
		inorder_traversed.size());

	std::cout << "Reconstructed tree \n LevelOrderTraversal "<<std::endl;
	LevelOrderTraversal(reconstructed_root);
	std::cout << "InorderTraverse ";
	std::cout<<InorderTraverse(reconstructed_root);
	std::cout<<std::endl;
	std::cout << "PostorderTraverse ";
	std::cout<<PostorderTraverse(reconstructed_root);
	std::cout<<std::endl;
	std::cout << "PreorderTraverse ";
	std::cout<<PreorderTraverse(reconstructed_root);
	std::cout<<std::endl;
	return 0;
}