#include <iostream>
#include <vector>
#include <chrono>
#include <random>

namespace {

template <class T>
class Matrix {
public:
	explicit Matrix(const int rows, const int cols) : rows_(rows), cols_(cols) {
		data_ = std::vector<std::vector<T>>(rows_, std::vector<T>(cols_, 0));
	}

	void RandInit(const T min = 0, const T max = 255) {
		unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
  		std::default_random_engine generator (seed);
  		for (unsigned int row = 0; row < rows_; row++) {
			for (unsigned int col = 0; col < cols_; col++) {
				std::uniform_int_distribution<T> distribution(min, max);
				data_[row][col] = distribution(generator);
			}
		}
	}

	void Print() {
		std::cout<<"Printing matrix"<<std::endl;
		std::cout<<"rows "<<rows_<<std::endl;
		std::cout<<"cols "<<cols_<<std::endl;
		
		for (const auto& row : data_) {
			for (const auto& element : row) {
				std::cout<<element<<" ";
			}
			std::cout<<std::endl;
		}
	}

	void FlipLR() {
		std::cout<<"Flipping matrix Left-Right "<<std::endl;
		for (int row = 0; row < rows_ ; row++) {
			for (int col = 0; col < cols_ / 2; col++) {
				auto temp = std::move(data_[row][col]);
				data_[row][col] = std::move(data_[row][cols_ - 1 - col]);
				data_[row][cols_ - 1 - col] = std::move(temp);
			}
		}
	}

	void FlipUD() {
		std::cout<<"Flipping matrix Upside-Down "<<std::endl;
		for (int row = 0; row < rows_ / 2; row++) {
			auto temp = std::move(data_[row]);
			data_[row] = std::move(data_[rows_ - 1 - row]);
			data_[rows_ - 1 - row] = std::move(temp);
		}
	}
protected:
	const int rows_;
	const int cols_;
	std::vector<std::vector<T>> data_;

};

template <class T>
class SquareMatrix : public Matrix<T> {
public:
	explicit SquareMatrix(int dimension) : Matrix<T>(dimension, dimension) {}
};

} //

int main(int argc, char** argv) {
	SquareMatrix<int> s(5);
	s.RandInit(0, 9);
	s.Print();
	s.FlipLR();
	s.Print();
	s.FlipUD();
	s.Print();
	return 0;
}