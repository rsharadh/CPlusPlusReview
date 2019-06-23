#include <iostream>
#include <vector>

namespace {
void Display(const std::vector<int> events) {
  for (const auto& e : events) {
    std::cout<<e<<" ";
  }
  std::cout<<std::endl;
}

std::vector<int> Add(const std::vector<int>& list_a, const std::vector<int>& list_b) {
	std::vector<int> list_c;
	for (const auto& a: list_a) {
		list_c.push_back(a);
	}
	for (const auto& b: list_b) {
		list_c.push_back(b);
	}
	return list_c;
}

std::vector<int> FindLongestIncreasingSubsequence(const std::vector<int>& list) {
	if (list.size() == 1) {
		return list;
	}
	int current_index = 0;
	int index = 0;
	while (true) {
		if (index == list.size() - 1) {
			break;
		}
		if (list[index] > list[current_index]) {
			index++;
		} else {
			break;
		}
	}

	return list;
}

} // namespace

int main(int argc, char** argv) {
	std::vector<int> list = {1, 3, 0, 7, 2, 10, 55, -1, 100};
	std::cout<<"list ";Display(Add(list, list));
	return 0;
}