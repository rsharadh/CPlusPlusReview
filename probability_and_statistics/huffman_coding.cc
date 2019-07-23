#include <iostream>
#include <iomanip>
#include <string>
#include <unordered_map>
#include <queue>
#include <vector>

namespace {

struct HuffmanNode {
	HuffmanNode(
		const float probability) : probability(probability), bit_value(false), parent(nullptr) {} 
	bool bit_value;
	float probability;
	HuffmanNode* parent;
};

class HuffmanComparator {
public:
	bool operator()(const HuffmanNode* lhs, const HuffmanNode* rhs) {
		return lhs->probability > rhs->probability;
	}
};

class AlphabetComparator {
public:
	bool operator()(const std::pair<std::string, float>& lhs, const std::pair<std::string, float>& rhs) {
		return lhs.second < rhs.second;
	}
};

std::string ParseHuffmanCode(HuffmanNode* node) {
	if (node == nullptr) {
		return "";
	}
	return ParseHuffmanCode(node->parent) + (node->bit_value ? "1" : "0");
 }

std::unordered_map<std::string, std::string> BuildHuffmanTree(
	const std::unordered_map<std::string, float>& alphabet_probabilities) {
	if (alphabet_probabilities.size() <= 2) {
		throw std::string("Need at least 3 elements to build a Huffman tree.");
	}
	std::unordered_map<std::string, std::string> huffman_codes;
	std::unordered_map<std::string, HuffmanNode*> alphabet_to_node;
	std::vector<HuffmanNode*> all_nodes;

	std::priority_queue<
		HuffmanNode*, 
		std::vector<HuffmanNode*>, 
		HuffmanComparator> huffman_q;
	for (const auto& kv : alphabet_probabilities) {
		alphabet_to_node[kv.first] = new HuffmanNode(kv.second);
		all_nodes.push_back(alphabet_to_node[kv.first]);
		huffman_q.push(alphabet_to_node[kv.first]);
	}

	while(true) {
		HuffmanNode* lowest = huffman_q.top();
		huffman_q.pop();
		lowest->bit_value = false;

		HuffmanNode* second_lowest = huffman_q.top();
		huffman_q.pop();
		second_lowest->bit_value = true;
		if (huffman_q.empty()) {
			break;
		}
		HuffmanNode* merged = new HuffmanNode(lowest->probability + second_lowest->probability);
		lowest->parent = merged;
		second_lowest->parent = merged;	
		huffman_q.push(merged);
		all_nodes.push_back(merged);
	}

	// Parse Huffman tree and get Huffman codes.
	for (const auto& kv : alphabet_to_node) {
		huffman_codes[kv.first] = ParseHuffmanCode(kv.second);
	}

	// Clear dynamically allocated memory.
	for (const auto& node: all_nodes) {
		delete node;
	}
	return huffman_codes;
}
}  // namespace

int main(int argc, char** argv) {
	std::unordered_map<std::string, float> alphabet_probabilities = 
	 {
	 	{"A", 0.5},
	 	{"B", 0.25},
	 	{"C", 0.125},
	 	{"D", 0.06},
	 	{"E", 0.05}, 
	 	{"F", 0.01},
	 	{"G", 0.005}, 
	 };

	std::priority_queue<
		std::pair<std::string, float>, 
		std::vector<std::pair<std::string, float>>, 
		AlphabetComparator> alphabet_q;
	for (const auto& kv : alphabet_probabilities) {
		alphabet_q.push(kv);
	}

	auto huffman_codes = BuildHuffmanTree(alphabet_probabilities);
	while (!alphabet_q.empty()) {
		const auto& alphabet = alphabet_q.top().first;
		const auto& probability	= alphabet_q.top().second;
		const auto& huffman_code = huffman_codes[alphabet];
		std::cout<<alphabet<<" "<<std::setw(6)
			<<probability<<" "<<huffman_code<<std::endl;
		alphabet_q.pop();
	}

	return 0;
}