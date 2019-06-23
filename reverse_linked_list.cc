#include <vector>
#include <string>
#include <iostream>

namespace {
struct Node {
	int value;
	Node* next;
};

void Display(Node* head) {
	while (head != nullptr) {
		std::cout<<head->value<<" ";
		head = head->next;
	}
	std::cout<<std::endl;
}
Node* Insert(Node* head, const int value) {
	Node* new_node = new Node();
	new_node->value = value;
	new_node->next = head;
	head = new_node;
	return head;
}

Node* Reverse(Node* head) {
	if (head == nullptr) {
		return nullptr;
	}
	Node* forward_iterator = head;
	Node* rev_head = nullptr;
	Node* reverse_iterator = rev_head; 
	while (forward_iterator != nullptr) {
		reverse_iterator = forward_iterator;
		forward_iterator = forward_iterator->next;
		reverse_iterator->next = rev_head;
		rev_head = reverse_iterator;
	}
	return rev_head;
}

int Length(Node* head) {
	int length = 0;
	while (head != nullptr) {
		length++;
		head = head->next;
	}
	return length;
}
}  // namespace

int main(int argc, char** argv) {
	Node* head = Insert(nullptr, 0);
	head = Insert(head, 1);
	head = Insert(head, 2);
	head = Insert(head, 3);
	Display(head);
	Node* rev_head = Reverse(head);
	Display(rev_head);
	std::cout<<"length "<<Length(rev_head)<<std::endl;
	return 0;
}