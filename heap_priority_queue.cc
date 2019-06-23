#include <iostream>
#include <vector>

namespace {
void Display(int* a, int length) {
  for (int index = 0; index < length; index++) {
	std::cout<<a[index]<<" ";	
  }
  std::cout<<std::endl;
}

void Swap(int& a, int& b) {
	int temp = a;
	a = b;
	b = temp;
}

void MaxHeapify(int* a, int length, int index) {
  int left_child_index = 2 * index + 1;
  if (left_child_index >= length) {
  	return;
  }
  int swap_index = index;
  if (a[left_child_index] > a[swap_index]) {
  	swap_index = left_child_index;
  }
  int right_child_index = left_child_index + 1;
  if (right_child_index < length) {
    if (a[right_child_index] > a[swap_index]) {
  	  swap_index = right_child_index;
    }
  }
  if (swap_index != index) {
  	Swap(a[swap_index], a[index]);
  	MaxHeapify(a, length, swap_index);
  }
}

void BuildMaxHeap(int* a, int length) {
	int start_index = length / 2;
	for (int index = start_index; index >=0; index--) {
		MaxHeapify(a, length, index);
	}
}

void HeapSort(int* a, int length) {
	BuildMaxHeap(a, length);
	for (; length > 0; length--) {
		Swap(a[0], a[length - 1]);
		MaxHeapify(a, length - 1, 0);
	}
}
}  // namespace
int main(int argc, char** argv) {
	int a[9] = {0, 1, 2, 5, 3, 6, 7, 8, 4};
	Display(a, 9);
	//BuildMaxHeap(a, 9);
	//Display(a, 9);
	HeapSort(a, 9);
	Display(a, 9);
	return 0;
}