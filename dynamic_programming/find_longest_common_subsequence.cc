#include <iostream>
#include <string>
#include <vector>
namespace {

struct LCS{
	int index_a;
	int index_b;
	bool is_match;
	int length;
	LCS* prev;
};

void PrintCommonStructure(
	const std::vector<std::string>& sequence_a,
	std::vector<std::string>& sequence_b,
	const std::vector<std::vector<LCS*>>&  common_structure) {
	for (int index_a = 0; index_a < sequence_a.size(); ++index_a) {
		for (int index_b = 0; index_b < sequence_b.size(); ++index_b) {
			std::cout<<common_structure[index_a][index_b]->is_match<<" ";
		}
		std::cout<<std::endl;
	}
}
std::string PrintLCS(
	const std::vector<std::string>& sequence_a,
	const std::vector<std::string>& sequence_b,
	LCS* lcs_ptr) {
  std::string prev_string;
  if (lcs_ptr == nullptr) {
  	return prev_string;
  }	
  prev_string = PrintLCS(
  	sequence_a, sequence_b, lcs_ptr->prev);
  if (lcs_ptr->is_match) {
  	prev_string += sequence_a[lcs_ptr->index_a] + " ";  	
  }
  return prev_string;
}
}  // namespace
int main(int argc, char** argv) {

	// std::vector<std::string> sequence_a = {"A", "B", "C"};
	// std::vector<std::string> sequence_b = {"A", "B", "C"};
	
	std::vector<std::string> sequence_a = 
	{ "A", "C", "X", "A", "D", "F", "B", "G" , "T", "C" , "A"}; 
	std::vector<std::string> sequence_b = 
	{  "G", "Y", "M", "A", "N", "Z", "T", "C" , "A"};

	// std::vector<std::string> sequence_a = {"A", "B", "C"};
	// std::vector<std::string> sequence_b = {"K", "B", "Z", "D", "E", "F", "C"};


	std::vector<std::vector<LCS*>>  common_structure;
	for (int index_a = 0; index_a < sequence_a.size(); ++index_a) {
		std::vector<LCS*> common_structure_a_b;
	for (int index_b = 0; index_b < sequence_b.size(); ++index_b) {
			LCS* lcs_ptr = new LCS();
			lcs_ptr->index_a = -1;
			lcs_ptr->index_b = -1;
			lcs_ptr->prev = nullptr;
			lcs_ptr->is_match = false;
			lcs_ptr->length = 0;
			common_structure_a_b.push_back(lcs_ptr);
		}
		common_structure.push_back(common_structure_a_b);
	}

	LCS* match_location = nullptr;
	int index_b = 0;
	bool found_match = false;
	for (int index_a = 0; index_a < sequence_a.size(); ++index_a) {
		if (sequence_a[index_a] == sequence_b[0]) {
			found_match = true;
			match_location = common_structure[index_a][0];
			match_location->index_a = index_a;
			match_location->index_b = index_b;
			match_location->is_match = true;
			match_location->length = 1;
			match_location->prev = nullptr;
		}
		if (found_match) {
			common_structure[index_a][0]->length = 1;	
			if (match_location != common_structure[index_a][0]) {
				common_structure[index_a][0]->prev = match_location;
			}
		}
	}

	match_location = nullptr;
	int index_a = 0;
	found_match = false;
	for (int index_b = 0; index_b < sequence_b.size(); ++index_b) {
		if (sequence_b[index_b] == sequence_a[0]) {
			found_match = true;
			match_location = common_structure[0][index_b];
			match_location->index_a = index_a;
			match_location->index_b = index_b;
			match_location->is_match = true;
			match_location->prev = nullptr;
		}
		if (found_match) {	
			common_structure[0][index_b]->length = 1;	
			if (match_location != common_structure[0][index_b]) {
				common_structure[0][index_b]->prev = match_location;
			}
		}
	}

	//PrintCommonStructure(sequence_a, sequence_b, common_structure);

	for (int index_a = 1; index_a < sequence_a.size(); ++index_a) {
		for (int index_b = 1; index_b < sequence_b.size(); ++index_b) {
			if (sequence_a[index_a] == sequence_b[index_b]) {
				common_structure[index_a][index_b]->index_a = index_a;
				common_structure[index_a][index_b]->index_b = index_b;
				common_structure[index_a][index_b]->is_match = true;
				common_structure[index_a][index_b]->length = 
					(common_structure[index_a-1][index_b-1]->length) + 1;		
				common_structure[index_a][index_b]->prev = 
					common_structure[index_a-1][index_b-1];
			} else {
				LCS* max_ptr = common_structure[index_a-1][index_b];
				if (common_structure[index_a][index_b-1]->length > 
					common_structure[index_a-1][index_b]->length) {
					max_ptr = common_structure[index_a][index_b-1]; 
				}
				common_structure[index_a][index_b]->length =
					max_ptr->length;
				common_structure[index_a][index_b]->prev = max_ptr;
			}
		}
	}

	std::cout<<"LCS length : "<<
		common_structure[sequence_a.size() - 1][
			sequence_b.size() - 1]->length<<std::endl;
	std::cout<<"LCS : "<<PrintLCS(sequence_a, sequence_b,
		common_structure[
			sequence_a.size() - 1][
			sequence_b.size() - 1])<<std::endl;
	return 0;
}