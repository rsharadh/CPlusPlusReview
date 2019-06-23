#include <iostream>
#include <vector>

namespace {
void Display(const std::vector<int> events) {
  for (const auto& e : events) {
    std::cout<<e<<" ";
  }
  std::cout<<std::endl;
}

int BinarySearchEquals(const std::vector<int> sorted_list, const int element) {
	int low = 0;
	int high = sorted_list.size() - 1;
	int mid = 0;
	while (low <= high) {
		mid = low + (high - low) / 2;
		std::cout<<low<<", "<<mid<<", "<<high<<std::endl;

		if (sorted_list[mid] == element) {
			return mid;
		}
		if (sorted_list[mid] < element) {
			low = mid + 1;
		} else {
			high = mid - 1;
		}
	}
	if (sorted_list[mid] == element) {
		return mid;
	}
	return -1;
}

int BinarySearchGreater(const std::vector<int> sorted_list, const int element) {
	int low = 0;
	int high = sorted_list.size() - 1;
	int mid = 0;
	while (low < high) {
		mid = low + (high - low) / 2;
		std::cout<<low<<", "<<mid<<", "<<high<<std::endl;

		if (sorted_list[mid] < element) {
			if ((mid + 1) <= sorted_list.size() - 1) {
				if ( sorted_list[mid + 1] >= element) {
					return mid + 1;
				} else {
					low = mid + 1;
				}
			} else {
				return -1;
			}
		} else {
			high = mid;
		}
	}
	if (((mid + 1) <= sorted_list.size() - 1) && sorted_list[mid + 1] >= element) {
			return mid + 1;
	}
	return -1;
}
} // namespace

int main(int argc, char** argv) {
	std::vector<int> sorted_list = {1, 2, 3, 10, 23, 34, 35, 90};

	int index = BinarySearchEquals(sorted_list, 34);
	if (index != -1) {
		std::cout<<"Found at index "<<index;
	} else {
		std::cout <<"Not found";
	}
	std::cout<<std::endl;

	Display(sorted_list);
	int element_to_insert = 96;
	int min_index = BinarySearchGreater(sorted_list, element_to_insert);
	if (min_index != -1) {
		std::cout<<"Insert "<<element_to_insert<<" at index "<<min_index;
	} else {
		std::cout <<"All less than "<<element_to_insert;
	}
	std::cout<<std::endl;
	return 0;
}
