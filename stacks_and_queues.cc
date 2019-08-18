#include <iostream>
#include <string>
#include <vector>

namespace {

template <class T>
struct Node {
	Node() : next(nullptr) {}
	Node(T val) : value(val), next(nullptr) {}
	T value;
	Node<T>* next;
};

template <class T>
class QueueWithLinkedList {
public:
	QueueWithLinkedList() : size_(0), capacity_(0), front_(nullptr), back_(nullptr) {}
	QueueWithLinkedList(const int capacity) : size_(0), capacity_(capacity), front_(nullptr), back_(nullptr) {}

	QueueWithLinkedList(const QueueWithLinkedList&) = delete;
	QueueWithLinkedList(QueueWithLinkedList&&) = delete;
	QueueWithLinkedList& operator=(const QueueWithLinkedList&) = delete;
	QueueWithLinkedList& operator=(QueueWithLinkedList&&) = delete;	

	bool IsEmpty() {
		return size_ == 0;
	}

	bool IsFull() {
		return size_ == capacity_;
	}

	void Print() {
		if (IsEmpty()) {
			throw std::string("QueueWithLinkedList is empty");
		}

		std::cout<<"Queue contents"<<std::endl;
		Node<T>* curr_ptr = front_;
		while (curr_ptr != nullptr) {
			std::cout<<curr_ptr->value<<std::endl;
			curr_ptr = curr_ptr->next;
		}
	}

	T PopFront() {
		if (IsEmpty()) {
			throw std::string("QueueWithLinkedList is empty");
		}
		T return_value = front_->value;
		front_ = front_->next;
		size_--;
		return return_value;
	}

	void PushBack(T val) {
		if (IsFull()) {
			throw std::string("QueueWithLinkedList is full");
		}

		size_++;
		if (front_ == nullptr) {
			front_ = new Node<T>(val);
			back_ = front_;
			return;
		}

		Node<T>* new_ptr = new Node<T>(val);
		back_->next = new_ptr;
		back_ = new_ptr;
	}
private:
	const int capacity_;
	int size_;
	Node<T>* front_;
	Node<T>* back_;
};

template <class T>
class StackWithArrays {
public:
	StackWithArrays() : capacity_(0), top_(0) {}
	StackWithArrays(const int capacity) : capacity_(capacity), top_(0) {
		array_.reserve(capacity_);
	}

	StackWithArrays(const StackWithArrays&) = delete;
	StackWithArrays(StackWithArrays&&) = delete;
	StackWithArrays& operator=(const StackWithArrays&) = delete;
	StackWithArrays& operator=(StackWithArrays&&) = delete;

	bool IsEmpty() {
		return array_.empty();
	}

	bool IsFull() {
		return array_.size() == capacity_;
	}

	void Push(T value) {
		if (IsFull()) {
			throw std::string("Stack is full");
		}
		array_.push_back(value);
		top_++;
	}

	T Peek() {
		if (IsEmpty()) {
			throw std::string("Stack is empty");
		}
		return array_[top_ - 1];
	}

	void Pop() {
		if (IsEmpty()) {
			throw std::string("Stack is empty");
		}
		array_.pop_back();
		top_--;
	}

	void Print() {
		if (IsEmpty()) {
			std::cout<< "Stack is empty" << std::endl;
			return;
		}
		std::cout<<"Stack contents "<<std::endl;
		for (int index = top_ - 1; index >= 0; index--) {
			std::cout<<array_[index]<<std::endl;
		}
	}
private:
	const int capacity_;
	int top_;
	std::vector<T> array_;
};
}  // namespace

int main(int argc, char** argv) {
	StackWithArrays<int> s(5);
	s.Push(1);
	s.Push(50);
	s.Push(3);
	s.Print();
	std::cout<<"Top of stack "<<s.Peek()<<std::endl;
	std::cout<<"Popping stack and pushing in a new value"<<std::endl;
	s.Pop();
	s.Push(100);
	s.Print();

	QueueWithLinkedList<int> q(5);
	q.PushBack(10);
	q.PushBack(-1);
	q.PushBack(200);
	q.Print();
	std::cout<<"Value popped from front of queue "<<q.PopFront()<<std::endl;
	q.Print();
	return -1;
}
