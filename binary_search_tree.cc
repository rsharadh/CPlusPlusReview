#include <vector>
#include <deque>
#include <string>
#include <iostream>

namespace {
struct Node {
	int value;
	Node* left;
	Node* right;
	Node* parent;
};

void PreorderDisplay(Node* root) {
	if (root == nullptr) {
		return;
	}
	std::cout<<root->value<<" ";
	PreorderDisplay(root->left);
	PreorderDisplay(root->right);
}
void PostorderDisplay(Node* root) {
	if (root == nullptr) {
		return;
	}
	PostorderDisplay(root->left);
	PostorderDisplay(root->right);
	std::cout<<root->value<<" ";
}

void InorderDisplay(const Node* root) {
	if (root == nullptr) {
		return;
	}
	InorderDisplay(root->left);
	std::cout<<root->value<<" ";
	InorderDisplay(root->right);
}

void Swap(
	std::deque<Node*>*& prim_queue_ptr, 
	std::deque<Node*>*& sec_queue_ptr) {
	std::deque<Node*>* temp = prim_queue_ptr;
	prim_queue_ptr = sec_queue_ptr;
	sec_queue_ptr = temp;
}

void LevelOrderDisplay(Node* root) {
	std::deque<Node*> primary_queue;
	std::deque<Node*>* prim_queue_ptr = &primary_queue;
	std::deque<Node*> secondary_queue;
	std::deque<Node*>* sec_queue_ptr = &secondary_queue;

	prim_queue_ptr->push_back(root);
	std::cout<<std::endl;

	while (true) {
		while (prim_queue_ptr->size() != 0) {
			Node* current_node = prim_queue_ptr->front();
			prim_queue_ptr->pop_front();
			std::cout<<current_node->value<<" ";
			if (current_node->left != nullptr) {
			  sec_queue_ptr->push_back(current_node->left);
			}
			if (current_node->right != nullptr) {
			  sec_queue_ptr->push_back(current_node->right);
			}
		}
		std::cout<<std::endl;
		Swap(prim_queue_ptr, sec_queue_ptr);
		if (prim_queue_ptr->size() == 0) {
			break;
		}	
	}
}

void Insert(Node*& root, Node* parent, const int value) {
	if (root == nullptr) {
		root = new Node();
		root->value = value;
		root->left = nullptr;
		root->right = nullptr;
		root->parent = parent;
		return;
	}
	if (value < root->value) {
		Insert(root->left, root, value);
	} else {
		Insert(root->right, root, value);
	}
}

Node* Min(Node* root) {
	if (root->left ==nullptr) {
		return root;
	}
	return Min(root->left);
}

Node* Max(Node* root) {
	if (root->right == nullptr) {
		return root;
	}
	return Max(root->right);
}

int Height(Node* root) {
	if (root == nullptr) {
		return 0;
	}
	return 1 + std::max(Height(root->left), Height(root->right));
}

Node* Predecessor(Node* root) {
	if (root == nullptr) {
		return nullptr;
	}
	if (root->left != nullptr) {
		return Max(root->left);
	} 

	Node* parent = root->parent;
	if (parent == nullptr) {
		return parent;
	}
	while (root == parent->left) {
		root = parent;
		parent = parent->parent;
	}
	return parent;
}

void SetPointerFromParent(Node* node, Node* new_destination) {
  Node* parent = node->parent;
  if (parent == nullptr) {
  	 return;
  }
  if (parent->left == node) {
	 parent->left = new_destination;
  } else {
	 parent->right = new_destination;
  }
}

Node* Search(Node* root, const int value) {
	if (root == nullptr) {
		return nullptr;
	}
	if (root->value == value) {
		return root;
	}
	if (value < root->value) {
		return Search(root->left, value);
	} else {
		return Search(root->right, value);
	}
}

void Delete(Node* node) {
	if ((node->left == nullptr) && (node->right ==nullptr)) {
		SetPointerFromParent(node, nullptr);
		delete node;
		return;
	}

	if (node->left == nullptr) {
		Node* parent = node->parent;
		if (parent != nullptr) {
			SetPointerFromParent(node, node->right);
		}
		node->right->parent = parent;
		delete node;
	} else {
		Node* predeccsor = Predecessor(node);
		if (predeccsor->left != nullptr) {
			SetPointerFromParent(predeccsor, predeccsor->left);
			predeccsor->left->parent = predeccsor->parent; 
		} else {
			SetPointerFromParent(predeccsor, nullptr);
		}
		node->value = predeccsor->value;
		delete predeccsor;
	}
}

}  // namespace

int main(int argc, char** argv) {
	Node* root = nullptr;
	Insert(root, nullptr, 200);
	Insert(root, nullptr, 3);
	Insert(root, nullptr, 101);
	Insert(root, nullptr, 201);
	Insert(root, nullptr, 13);
	Insert(root, nullptr, 210);
	Insert(root, nullptr, 250);
	Insert(root, nullptr, -3);
	Insert(root, nullptr, 100);
	std::cout<<"Inorder Display \n";
	InorderDisplay(root);
	std::cout<<std::endl;
	std::cout<<"Preorder Display \n";
	PreorderDisplay(root);
	std::cout<<std::endl;
	std::cout<<"Postorder Display \n";
	PostorderDisplay(root);
	std::cout<<std::endl;
	std::cout<<"LevelOrder Display \n";
	LevelOrderDisplay(root);

	std::cout<<"Tree min "<<Min(root)->value<<std::endl;
	std::cout<<"Tree max "<<Max(root)->value<<std::endl;
	std::cout<<"Root value "<<root->value<<std::endl;
	std::cout<<"Tree predeccsor of root "<<Predecessor(root)->value<<std::endl;

	int query = -3;
	Node* query_node = Search(root, query);
	if (query_node == nullptr) {
		std::cout<<"query value "<<query<<" not found int tree "<<std::endl;
	} else {
		std::cout<<"query value "<<query<<" found and now being deleted "<<std::endl;
		Delete(query_node);
		std::cout << "Inorder Display ";	
		InorderDisplay(root); std::cout<<std::endl;
	}
	std::cout<<"LevelOrder Display ";
	LevelOrderDisplay(root);
	std::cout<<"Tree height "<<Height(root)<<std::endl;
	return 0;
}