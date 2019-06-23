#include <iostream>
#include <string>
#include <exception>
#include <bitset>

namespace {
struct MyException : public std::exception {
public:
	const std::string message_;
	MyException(const std::string& message) : message_(message) {}

	const char* what() const noexcept {return message_.c_str();}
};

void ResetBits(
	int* first_number, const int second_number, 
	const int begin_pos, const int end_pos) {
   if (end_pos < begin_pos) {
   	throw MyException("end < begin pos");
   }
   int num_bits = end_pos - begin_pos + 1;
   if ((second_number >> num_bits) > 0) {
   	throw MyException("second_number needs to be smaller than 2^(end_pos - begin_pos + 1).");
   }

   int second_number_left_shifted = second_number << begin_pos;

   int all_ones = ~0;
   std::cout << "all_ones "<<all_ones<<std::endl;
   int all_ones_upto_begin = (1 << begin_pos) - 1;
   int all_ones_upto_end = (1 << end_pos) - 1;
   int all_ones_after_end = all_ones ^ all_ones_upto_end;
   int mask = all_ones_after_end | all_ones_upto_begin;

   int masked_first_num = *first_number & mask;
   *first_number = masked_first_num | second_number_left_shifted;
}

int CountNumberOfBits(int n) {
	int num_bits = 0;
	while (n > 0) {
		num_bits += (1 & n);
		n = n>>1;
	}
	return num_bits;
}

int SwapEvenOdd(const int n) {
	int odd_mask = 2;
	int intermediate_mask = 0;
	while (odd_mask > intermediate_mask) {
		intermediate_mask = odd_mask;
		odd_mask <<= 2;
		odd_mask += 2; 	
	}

	int even_mask = odd_mask >> 1;
	int odd_bits = odd_mask & n;
	int even_bits = even_mask & n;
	return (odd_bits >> 1)  + (even_bits << 1);
}
}  // namespace

int main(int argc, char** argv) {
	std::cout<<"Hello World"<<std::endl;
	int n = 32;
	int m = 2;
	int i = 3;
	int j = 5;
	std::cout<<"BEFORE: first number " << n <<" second number " << m <<std::endl;
	std::cout<<"BEFORE: first number " << std::bitset<32>(n) <<" second number " << std::bitset<32>(m) <<std::endl;
	ResetBits(&n, m, i, j);
	std::cout<<"AFTER: first number " << n <<" second number " << m <<std::endl;
	std::cout<<"AFTER: first number " << std::bitset<32>(n) <<" second number " << std::bitset<32>(m) <<std::endl;
	
	n = 23;
	std::cout<<"Count number of bits in "<<n<<" is "<<CountNumberOfBits(n)<<std::endl;
	std::cout << "Swap odd-even bits of "<<n<< "("<<std::bitset<32>(n)<<") = "<<SwapEvenOdd(n)<<"("<<std::bitset<32>(SwapEvenOdd(n))<<") \n";
	return 0;
}

/// A B C D
/// E F G H
/// 1 2 3 4
/// 5 6 7 8

// D H 4 8
// C G 3 7
// B F 2 6
// A E 1 5

// swap(a[i][j], a[j][i])
// A E 1 5 
// B F 2 6
// C G 3 7
// D H 4 8

// swap(a[i][j],a[N-1-i][j])
// D H 4 8 
// C G 3 7
// B F 2 6
// A E 1 5