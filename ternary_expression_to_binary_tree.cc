#include <iostream>
#include <vector>
#include <deque>
#include <stack>
#include <string>
#include <unordered_map>

namespace {
struct Node {
	Node(const std::string& expression) 
		: expression(expression), left(nullptr), right(nullptr) {}
	std::string expression;
	Node* left;
	Node* right;
}; 

// a?b:c
// a?b?c:d:e
// ? - 1, 3
// : - 5, 7 
// a?b:c?d:e
// ? - 1,5
// : - 3,7
// a?b?c?d:e:f:h?g:i
// ? - 1,3,5,13
// : - 7,9,11,15
// 1:11

void LevelOrderTraversal(Node* root) {
	if (root == nullptr) {
		return;
	}
	std::deque<Node*> current_queue;
	std::deque<Node*> next_queue;
	current_queue.push_back(root);
	while (!current_queue.empty() || !next_queue.empty()) {
		while (!current_queue.empty()) {
			auto current_node = current_queue.front();
			std::cout<<current_node->expression<<" ";
			if (current_node->left != nullptr) {
				next_queue.push_back(current_node->left);
			}
			if (current_node->right != nullptr) {
				next_queue.push_back(current_node->right);
			}
			current_queue.pop_front();
		}
		std::cout<<std::endl;
		current_queue = std::move(next_queue);
		next_queue.clear();
	}
	std::cout<<std::endl;
}

void InOrderTraversal(Node* root) {
	if (root == nullptr) {
		return;
	}
	InOrderTraversal(root->left);
	std::cout<<root->expression<<" ";
	InOrderTraversal(root->right);
}	

void PreOrderTraversal(Node* root) {
	if (root == nullptr) {
		return;
	}
	std::cout<<root->expression<<" ";
	PreOrderTraversal(root->left);
	PreOrderTraversal(root->right);
}	

void GetStats(const std::vector<std::string>& expression, std::vector<int>* sorted_question_pos, std::unordered_map<int, int>* question_to_colon) {
	std::stack<int> question_pos;
	for (unsigned int index = 0; index < expression.size(); index++) {
		const auto& value = expression[index];
		if (value == "?") {
			question_pos.push(index);
			sorted_question_pos->push_back(index);
		}
		if (value == ":"){
			const auto last_qn_index = question_pos.top();
			question_pos.pop();
			(*question_to_colon)[last_qn_index] = index;
		}
	}
}

// Can be optimized with binary search.	
int FindLargestIndexLessThan(
	const std::vector<int>& sorted_vals, 
	const int query) {
	int index = 0;
	for (; index < sorted_vals.size() - 1; index++) {
		if ((sorted_vals[index] <= query) 
			&& (sorted_vals[index + 1 ] > query)) {
			break;
		} 
		if (sorted_vals[index] > query) {
			break;
		}
	}
	return sorted_vals[index] < query ? index : -1 ;
}

Node* ParseTree(
	const std::vector<std::string>& expression,
	const std::vector<int>& sorted_question_pos,
	const std::unordered_map<int, int>& question_to_colon) {
	if (expression.size() < 5) { 
		return nullptr;
	}
	int root_question_pos = sorted_question_pos[0];
	//std::cout<<"root_question_pos "<<root_question_pos<<std::endl;
	int root_colon_pos = question_to_colon.at(root_question_pos);
	//std::cout<<"root_colon_pos "<<root_colon_pos<<std::endl;
	Node* root = new Node(expression[root_question_pos - 1]);

	int largest_question_index_less_than = FindLargestIndexLessThan(
		sorted_question_pos, root_colon_pos);
	//std::cout<<"largest_question_index_less_than "<<largest_question_index_less_than<<std::endl;
	if (root_colon_pos - root_question_pos > 5 && 
		largest_question_index_less_than != -1) {
		//std::cout<<"recursing left  "<<std::endl;
		root->left = ParseTree(
			expression, 
			std::vector<int>(
				sorted_question_pos.begin() + 1,
				sorted_question_pos.begin() + largest_question_index_less_than + 1), 
			question_to_colon);
	} else {
		root->left = new Node(expression[root_question_pos + 1]);
	}
	//std::cout<<"expression.size() "<<expression.size()<<std::endl;
	if (largest_question_index_less_than != -1 && 
		largest_question_index_less_than + 1 < sorted_question_pos.size()) {
		//std::cout<<"recursing right  "<<std::endl;
		root->right = ParseTree(
			expression, 
			std::vector<int>(
				sorted_question_pos.begin() + largest_question_index_less_than + 1,
				sorted_question_pos.end()), 
			question_to_colon);
	} else {
		root->right = new Node(expression[root_colon_pos + 1]);
	}
	return root;
}

}  // namespace

int main(int argc, char** argv) {

	std::vector<std::vector<std::string>> expressions = {
		{"a","?","b",":","c"},
	    {"a","?","b","?","c",":","d",":","e"},
	    {"a","?","b","?","c","?", "f", ":","d",":","e", ":", "h"},
	    {"a","?","b",":","c","?","d",":","e"},
	};
    for (const auto& expression : expressions) {
    	for (const auto& value: expression) {
    		std::cout<<value<<" ";
    	}
    	std::cout<<std::endl;
        std::vector<int> sorted_question_pos;
    	std::unordered_map<int, int> question_to_colon;
    	GetStats(expression,
    			 &sorted_question_pos,
    	 		 &question_to_colon);

    	// std::cout<<"question_to_colon "<<std::endl;
    	// for (const auto& kv : question_to_colon) {
    	// 	std::cout<<kv.first<<" "<<kv.second<<std::endl;
    	// }
    	Node* root = ParseTree(expression, sorted_question_pos, question_to_colon);
    	std::cout<<"LevelOrderTraversal "<<std::endl;
    	LevelOrderTraversal(root);
    	std::cout<<"PreOrderTraversal "<<std::endl;
    	PreOrderTraversal(root);
    	std::cout<<"\nInOrderTraversal "<<std::endl;
    	InOrderTraversal(root);
    	std::cout<<std::endl;
    	std::cout<<std::endl;
    }
	return 0;
}