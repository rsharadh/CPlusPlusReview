#include <iostream>
#include <string>

namespace {

struct Node {
	char val;
	Node* next;
};

void Display(Node* head) {
	if (head == nullptr) {
		std::cout<<std::endl;
		return;
	}
	std::cout<<head->val<<" ";
	Display(head->next);
}

Node* ReverseLinkedList(Node* head) {
	if (head == nullptr) {
		return head;
	}
	Node* forward_iterator = head;
	Node* reverse_iterator = nullptr;
	Node* reverse_head = nullptr;

	while (forward_iterator != nullptr) {
		reverse_iterator = forward_iterator;
		forward_iterator = forward_iterator->next;
		reverse_iterator->next = reverse_head;
		reverse_head = reverse_iterator;	
	}
	return reverse_head;
}

Node* Create(Node* head, char c) {
	Node* node = new Node();
	node->val = c;
	node->next = head;
	head= node;
	return head;
}
}  // namespace

int main(int argc, char** argv) {
	std::string test_string = "test value";
	for (const auto& c : test_string) {
		std::cout<<c<<" < > ";
	}
	std::cout<<std::endl;
	Node* head = Create(nullptr, 'c');
	head = Create(head, 'h');
	head = Create(head, 'a');
	head = Create(head, 'r');
	Display(head);
	head = ReverseLinkedList(head);
	Display(head);
	return 0;
}