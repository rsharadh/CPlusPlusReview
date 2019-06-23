#include <iostream>
#include <vector>
#include <unordered_map>
#include <unordered_set>

namespace {
void Display(const std::vector<int> events) {
  for (const auto& e : events) {
    std::cout<<e<<" ";
  }
  std::cout<<std::endl;
}

int FindLongestLessThan(
	const std::vector<std::vector<int>>& candidates_sequences, 
	const int upper_bound) {
	int max_length_index = -1;
	int max_length = 0;
	int index = 0;
	for (const auto& candidate_sequence: candidates_sequences) {
		if (candidate_sequence.back() <= upper_bound) {
			if (candidate_sequence.size() >= max_length) {
				max_length = candidate_sequence.size();
				max_length_index = index;
			}
		}
		index++;
	}
	return max_length_index;
}

std::vector<std::vector<int>> FindLongestIncreasingSubsequence(const std::vector<int>& list) {
	if (list.size() == 1) {
		return {list};
	}
	std::vector<std::vector<int>> increasing_subsequences_n;
	std::vector<std::vector<int>> increasing_subsequences_n_minus_1 = 
		FindLongestIncreasingSubsequence(std::vector<int>(list.begin(), list.end() - 1));
		
	int max_length_index = FindLongestLessThan(
		increasing_subsequences_n_minus_1, list.back());

	if (max_length_index == -1) {
		increasing_subsequences_n_minus_1.push_back({list.back()});
	} else {
		auto longest_sequence_less_than = increasing_subsequences_n_minus_1[max_length_index];
		longest_sequence_less_than.push_back(list.back());
		increasing_subsequences_n_minus_1.push_back(longest_sequence_less_than);
	}

	if (true) {
		// Pruning sequences that cant be optimal.
		std::unordered_multimap<unsigned int, const std::vector<int>*> length_to_sequence_pointers;
		std::unordered_set<int> unique_lengths;
		for (const auto& sequence : increasing_subsequences_n_minus_1) {
			length_to_sequence_pointers.insert({sequence.size(), &sequence});
			unique_lengths.insert(sequence.size());
		}
		for (const auto& length : unique_lengths) {
			const std::vector<int>* chosen_sequence_ptr = nullptr;
			const auto& range = length_to_sequence_pointers.equal_range(length);
			for (
				auto it = range.first; 
				it != range.second; 
				it++) {
				if (chosen_sequence_ptr == nullptr) {
					chosen_sequence_ptr = it->second;
				} else {
					if (it->second->back() < chosen_sequence_ptr->back()) {
						chosen_sequence_ptr = it->second;
					}
				}
			}
			increasing_subsequences_n.push_back(*chosen_sequence_ptr);
		}
	} else {
		// Less efficient
		increasing_subsequences_n = std::move(increasing_subsequences_n_minus_1);
	}
	return increasing_subsequences_n;
}

} // namespace

int main(int argc, char** argv) {
	std::vector<int> list = {1, 3, 0, 7, 2, 10, 55, -1, 100, 3, 4, 5, 6, 7, 7, 8, 9};
	//std::vector<int> list = {1, 2, 3, 4};

	std::cout<<"list ";Display(list);

	const auto& increasing_subsequences = FindLongestIncreasingSubsequence(list);
	std::cout<<"Longest increasing subsequence ";
	Display(increasing_subsequences[FindLongestLessThan(increasing_subsequences, 1e9)]);
	return 0;
}