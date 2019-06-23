/*

        3

       / \

      4   9

     /   / \

    5   4   6

       /

      5

Output:   [[4,5], [5]]

5 4 3 5 4 9 6

5
5 4
5 4 3
5
5 4
5 4 9
-> 5 4 3 5 4 9


3
 \
  4
   \
    3
     \
      4
*/


#include <iostream>
#include <string>
#include <unordered_map>

using namespace std;

namespace {
struct Node {
  int value;
  Node* left;
  Node* right;
};

class SubTreeMatch {  
  
  void Clear() {
    inorder_string_map.clear();
  }
  
  string InorderTraverseAndMatch(Node* root) {
    if (root == nullptr) {
      return;
    }
    string inorder_string, inorder_string_left, inorder_string_right; 
    if (root->left == nullptr) {
      inorder_string_left = InorderTraverseAndMatch(root->left);
    }
    if (root->right == nullptr) {
      inorder_string_right = InorderTraverseAndMatch(root->right);
    }
    inorder_string = inorder_string_left;
    inorder_string += std::to_string(root->value);
    inorder_string += inorder_string_right;
    inorder_string_map[inorder_string] = root;
    return inorder_string;
  }
  
  void PrintMatches() {
    for (unsigned int i = 0; i <= inorder_string_map.bucket_count(); i++) {
      for (auto it = inorder_string_map.begin(i); 
           it != inorder_string_map.end(i); ++it) {
        // traverse through all matches for the current bucket key
      }
    }
  }  
  private:
  std::unordered_multimap<string , Node*> inorder_string_map; 
};  

// To execute C++, please define "int main()"
int main() {
  auto words = { "Hello, ", "World!", "\n" };
  for (const string& word : words) {
    cout << word;
  }
  return 0;
}

