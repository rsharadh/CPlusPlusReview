#include <iostream>
#include <stack>
#include <string>
#include <vector>

namespace {

bool is_operator(const char c) {
	switch (c) {
		case '+':
		case '-':
		case '*':
		case '/':
			return true;
		default:
			return false;
	}
}

enum Operator {
	PLUS = 0,
	MINUS,
	MUL,
	DIV
};

Operator GetOperator(const char c) {
	switch (c) {
		case '+':
			return PLUS;
		case '-':
			return MINUS;
		case '*':
			return MUL;
		case '/':
		default:
			return DIV;
	}
}

char GetOperator(const Operator op) {
	switch (op) {
		case PLUS :
			return '+';
		case MINUS:
			return '-';
		case MUL:
			return '*';
		case DIV:
		default:
			return '/';
	}
}

bool IsHigherPrecedence(const Operator a, const Operator b) {
	if (a == b){
		return true;
	}
	return a > b;
}

template <class T>
T Apply(Operator op, T a, T b){
	switch (op) {
		case PLUS:
			return a + b;
		case MINUS:
			return b - a;
		case MUL:
			return a * b;
		case DIV:
		default:
			return b / a;
	}
}

void Print(const Operator op) {
	switch (op) {
		case PLUS:
			std::cout <<"Operator PLUS"<<std::endl;
			break;
		case MINUS:
			std::cout <<"Operator MINUS"<<std::endl;
			break;
		case MUL:
			std::cout <<"Operator MUL"<<std::endl;
			break;
		case DIV:
		default:
			std::cout <<"Operator DIV"<<std::endl;
			break;
	}
}

// Based on Shunting-Yard algorithm by djikstra
// https://en.wikipedia.org/wiki/Shunting-yard_algorithm#A_simple_conversion
template <class T>
std::string InfixToPostfix(const std::string& infix_expression) {
	std::stack<char> output_stack;
	std::stack<char> op_stack;
	std::string postfix_expression;

	for (auto c : infix_expression) {
		if (is_operator(c)) {
			if (op_stack.empty() 
				|| IsHigherPrecedence(
					GetOperator(c), 
					GetOperator(op_stack.top()))) {
				op_stack.push(c);
			} else {
				while (!op_stack.empty() 
					&& IsHigherPrecedence(
						GetOperator(op_stack.top()), 
						GetOperator(c))) {
					postfix_expression += std::string(1, op_stack.top());
					op_stack.pop();
				}
				op_stack.push(c);
			}
		} else {
			postfix_expression += std::string(1, c);
		}
	}

	postfix_expression += " ";
	while (!op_stack.empty()) {
		postfix_expression += std::string(1, op_stack.top());
		postfix_expression += " ";
		op_stack.pop();
	}
	return postfix_expression;
}

// Based on postfix (reverse Polish notation) eval,
// https://www.geeksforgeeks.org/stack-set-4-evaluation-postfix-expression/
// or http://faculty.cs.niu.edu/~hutchins/csci241/eval.htm 
template <class T>
T EvaluatePostfix(const std::string& postfix_expression) {
	std::stack<T> operands;

	for (const auto c : postfix_expression) {
		if (c == ' ') {
			continue;
		}
		if (is_operator(c)) {
			auto this_op = GetOperator(c);
			auto a = operands.top(); 
			operands.pop();
			auto b = operands.top(); 
			operands.pop(); 
			T result = Apply(this_op, a, b);
			//std::cout<<a<<" ";Print(top_op);std::cout" "<<b<<" = "<<result<<std::endl;
			operands.push(result);

		} else {
			if (c > '9' || c < '0') {
				throw std::string("invalid expression");
			}
			//std::cout<<"operand "<<c - '0'<<std::endl;
			operands.push(c - '0');
		}
	}
	if (operands.size() != 1) {
		throw std::string("invalid expression");
	}
	return operands.top();
}

}  // namespace

int main(int argc, char** argv) {
	std::vector<std::string> postfix_expressions = {
		"2 3 +",
		"3 4 -",
		"2 4 *",
		"6 6 *",
		"6 6 /"};
	for (auto& postfix_expression : postfix_expressions){
		int result = EvaluatePostfix<int>(postfix_expression);
		std::cout<<postfix_expression<<" evaluates as "<<result<<std::endl;
	}

	std::cout<<std::endl;
	std::vector<std::string> infix_expressions = {
		"2 + 3 / 3",
		"2 + 4 * 3",
		"3 - 4",
		"6 * 6",
		"6 / 6",
		"2 * 4 + 6",};
	for (auto& infix_expression : infix_expressions){
		auto postfix_expression = InfixToPostfix<int>(infix_expression);
		std::cout<<infix_expression<<" in post-fix as "<<postfix_expression<<std::endl;
		int expr_result = EvaluatePostfix<int>(postfix_expression);
		std::cout<<postfix_expression<<" evaluates as "<<expr_result<<std::endl;
	}
	return 0;
}
