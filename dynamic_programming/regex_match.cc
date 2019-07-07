#include <iostream>
#include <string>
#include <vector>

namespace {

bool IsRegexMatch(const std::string& text, const std::string& pattern) {
	// is_match_at[i][j] represents matching with the first i text
	// characters and the first j pattern characters. 
	std::vector<std::vector<bool>> is_match_at;
	for (int i = 0; i <= text.size(); i++) {
		is_match_at.push_back(
			std::vector<bool>(
				pattern.size() + 1, false));
	}

	// text and pattern are both null.
	is_match_at[0][0] = true;


	// pattern is null.
	for (unsigned int i = 1; i <= text.size(); i++) {
		is_match_at[i][0] = false;
	}

	// text is null
	for (unsigned int j = 1; j <= pattern.size(); j++) {
		unsigned int pattern_index = j - 1;
		is_match_at[0][j] = 
			pattern[pattern_index] == '*' 
			? is_match_at[0][j-1]
			: false;	
	}

	for (unsigned int i = 1; i <= text.size(); i++) {
		for (unsigned int j = 1; j <= pattern.size(); j++) {
			unsigned int pattern_index = j - 1;
			unsigned int text_index = i - 1;
			switch(pattern[pattern_index]) {
				case '?': 
					// '?' matches current character =>
					// is_match_at[i][j] == true if is_match_at[i-1][j-1] is true
					// is_match_at[i][j] == false if is_match_at[i-1][j-1] is false
					is_match_at[i][j] = is_match_at[i-1][j-1];
					break;
				case '*' :
					is_match_at[i][j] = 
						is_match_at[i][j-1] /* '*' matches empty string */
						|| is_match_at[i-1][j] /* '*' matches current and earlier characters */;
					break;
				default :
					// pattern[pattern_index] is a text character.
					is_match_at[i][j] = 
						(text[text_index] == pattern[pattern_index]) 
						&& is_match_at[i-1][j-1];
			}
		}
	}

	return is_match_at[text.size()][pattern.size()];
}
} // namespace 

int main(int argc, char** argv) {
	std::vector<std::string> patterns = {
		"a?b*a",
		"ba*****ab", 
		"ba*ab", 
		"a*ab",
		"a*****ab",
		"*a*****ab", 
		"*a*****", 
		"ba*ab****", 
		"****", 
		"*",
		"aa?ab", 
		"b*b", 
		"a*a", 
		"baaabab",
		"?baaabab", 
		"*baaaba*",
		"*bba*"
	}; 

	std::string text = "bba";
	for (const auto& pattern : patterns) {
		std::cout<< 
		(IsRegexMatch(text, pattern) 
			? (text + " is matched by " + pattern + "\n") 
			: (text + " is NOT matched by " + pattern + "\n"));
	}
	return 0;
}