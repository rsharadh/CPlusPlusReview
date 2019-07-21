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

protected:
	const int rows_;
	const int cols_;
	std::vector<std::vector<T>> data_;

};

template <class T>
class SquareMatrix : public Matrix<T> {
public:
	explicit SquareMatrix(int dimension) : Matrix<T>(dimension, dimension) {}

	void RotateCCW() {
		std::cout<<"Rotating matrix counter clock wise "<<std::endl;
		// Note end range of layers (it is exactly half the number
		// of rows - 1)
		for (int layer = 0; layer < this->rows_ / 2; layer++) {
			//std::cout<<"rotating layer "<<layer<<std::endl;
			// Note end range of index (it is one less than number
			// of rows/cols in each layer).
			for (int index = layer; 
				index < this->rows_ - layer - 1; 
				index++) {
				//std::cout<<"index "<<index<<std::endl;
				T temp = this->data_[index][layer];
				//std::cout<<index<<", "<<layer<<" <- ";
				this->data_[index][layer] = 
					this->data_[layer][this->cols_ - 1 - index];
				//std::cout<<layer<<", "<<this->cols_ - 1 - index<<" <- ";	
				this->data_[layer][this->cols_ -  1 - index] = 
					this->data_[this->rows_ -  1 - index][this->cols_ - layer - 1];
				//std::cout<<this->rows_ -  1 - index<<", "<<this->cols_ - layer - 1<<" <- ";	
				this->data_[this->rows_ -  1 - index][this->cols_ - layer - 1] =	
					this->data_[this->rows_ - layer - 1][index];
				//std::cout<<this->rows_ - layer - 1<<", "<< index<<""<<std::endl;	
				this->data_[this->rows_ - layer - 1][index] = temp;	
			}
		}
	}
};

} //

int main(int argc, char** argv) {
	SquareMatrix<int> s(5);
	s.RandInit(0, 9);
	s.Print();
	s.RotateCCW();
	s.RotateCCW();
	s.RotateCCW();
	s.RotateCCW();
	s.Print();
	return 0;
}