#include <iostream>
#include <vector>

namespace {
void Display(const std::vector<int>& array) {
	for (const auto& value : array) {
		std::cout<<value<<" ";
	}
	std::cout<<std::endl;
}	
std::vector<int> ComputeHistogram(const std::vector<int>& array, const int max_val) {
	std::vector<int> histogram(max_val + 1, 0);
	for (const auto& value: array) {
		histogram[value]++;
	}
	return histogram;
}

std::vector<int> CountingSort(const std::vector<int>& array, int max_value) {
	auto histogram = ComputeHistogram(array, max_value);
	// Display(histogram);
	int sum = 0;
	for (auto& h : histogram) {
		sum += h;
		h = sum;
	}
	std::vector<int> sorted_array(array.size(), 0);
	for (int index = array.size() - 1; index >= 0; index--) {
		int value = array[index];
		int landing_index = histogram[value] - 1;
		sorted_array[landing_index] = value;	
		histogram[value]--;
	}
	return sorted_array;
}

}  // namespace

int main(int argc, char** argv) {
	std::vector<int> array = {9 ,8 , 7 ,7 ,6 ,5, 4, 3, 2, 0};
	Display(array);
	const auto& sorted_array = CountingSort(array, 9);
	Display(sorted_array);
}
