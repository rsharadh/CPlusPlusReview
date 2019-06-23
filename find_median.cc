#include <iostream>
#include <string>
#include <vector>
#include <stdlib.h>     /* srand, rand */


namespace {
void Display(const std::vector<int> events) {
  for (const auto& e : events) {
    std::cout<<e<<" ";
  }
  std::cout<<std::endl;
}

void Swap(int& a, int& b) {
	int temp = a;
	a = b;
	b = temp;
}

int Partition(int* a, int length, int pivot) {
	int pivot_index = 0;
	for (int i = 0; i < length; i++) {
		if (a[i] < pivot) {
			Swap(a[i], a[pivot_index]);
			pivot_index++;
		}
	}
	return pivot_index;
}

int FindMedian(int* a, int length, int ord_stat_location) {
 if (ord_stat_location > length - 1) {
   return -1;
 }
 if (length < 1) {
 	return -1;
 }
 int pivot_element_index = rand() % length;
 Swap(a[pivot_element_index], a[length -1]);
 int pivot_index = Partition(a, length, a[length - 1]);
 Swap(a[length - 1], a[pivot_index]);

 if (pivot_index < ord_stat_location) {
 	return FindMedian(
 		a + pivot_index, 
 		length - pivot_index, 
 		ord_stat_location - pivot_index);
 }
 if (pivot_index > ord_stat_location) {
 	return FindMedian(a, pivot_index, ord_stat_location);
 }
 return a[pivot_index];
}

} // namespace

int main(int argc, char** argv) {
	std::vector<int> list = {1, 3, 0, 7, 2, 10, 55, -1, 100};
	std::cout<<"list ";Display(list);
	int median = FindMedian(&list[0], list.size(), list.size() / 2);
	std::cout<<"median "<<((median != -1) ? std::to_string(median) : "not found")<<std::endl;
	std::cout<<"list ";Display(list);
	return 0;
}