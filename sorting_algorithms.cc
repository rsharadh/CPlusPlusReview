#include <iostream>
#include <vector>
#include <stdlib.h>     /* srand, rand */

namespace {
void Display(const std::vector<int> events) {
  for (const auto& e : events) {
    std::cout<<e<<" ";
  }
  std::cout<<std::endl;
}

void InsertionSort(int* a, int length) {
 for (int i = length - 1; i > 0; i--) {
 	int key = a[i];
 	int j = i - 1;
 	for (; j >=0; j--) {
 		if (a[j] >= key) {
 			a[j+1] = a[j];
 		} else {
 			break;
 		}
 	}
 	a[j + 1] = key;	
 }
}

void Swap(int& a, int& b) {
	int temp = a;
	a = b;
	b = temp;
}

void BubbleSort(int* a, int length) {
 for (int i = 0; i < length; i++) {
 	int min_val = a[i];
 	int min_ind = i;
 	for (int j = i + 1; j < length; j++) {
 		if (a[j] < min_val) {
 			min_val = a[j];
 			min_ind = j;
 		}	
 	}
	Swap(a[i], a[min_ind]);
 }
}

int Partition(int* a, int length, int pivot) {
	int pivot_index = 0;
	for (int i = 0; i < length ; i++) {
		if (a[i] < pivot) {
			Swap(a[i], a[pivot_index]);
			pivot_index++;
		}
	}
	return pivot_index;
}

void QuickSort(int* a, int length) {
 if (length < 1) {
 	return;
 }
 int pivot_element_index = rand() % length;
 Swap(a[pivot_element_index], a[length -1]);
 int pivot_index = Partition(a, length, a[length - 1]);
 Swap(a[length - 1], a[pivot_index]);

 QuickSort(a, pivot_index);
 if (pivot_index + 1 < length) {
 	QuickSort(a + pivot_index + 1, length - pivot_index - 1);
 }
}

} // namespace

int main(int argc, char** argv) {
	std::vector<int> list = {1, 3, 0, 7, 2, 10};
	std::cout<<"list ";Display(list);
	//InsertionSort(&list[0], list.size());
	//BubbleSort(&list[0], list.size());
	QuickSort(&list[0], list.size());
	std::cout<<"sorted list ";Display(list);
	return 0;
}