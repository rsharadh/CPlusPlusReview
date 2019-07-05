#include <iostream>
#include <iomanip>
#include <chrono>
#include <random>
#include <vector>

namespace {
constexpr float kMinMatrixValue = 0.01;

template<class T>
class Matrix {
public:
	Matrix() : width_(0), height_(0) {};
	Matrix(const int width, const int height) 
		: width_(width), height_(height) {
		for (int row = 0; row < height_; row++) {
			data_.push_back(std::vector<T>(width_, 0));
		}
	}

	int Height() const { 
		return height_;
	}

	int Width() const { 
		return width_;
	}

	T At(int row, int col) const {
		if (row < 0 && row >= height_) {
			throw std::string("Row outside range");
		}
		if (col < 0 && col >= width_) {
			throw std::string("Col outside range");
		}
		return data_[row][col];
	}


	T& At(int row, int col) {
		if (row < 0 && row >= height_) {
			throw std::string("Row outside range");
		}
		if (col < 0 && col >= width_) {
			throw std::string("Col outside range");
		}
		return data_[row][col];
	}

	void RandInit(const T min = 0, const T max = 255) {
		unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
  		std::default_random_engine generator (seed);
  		for (unsigned int row = 0; row < height_; row++) {
			for (unsigned int col = 0; col < width_; col++) {
				std::uniform_int_distribution<T> distribution(min, max);
				data_[row][col] = distribution(generator);
			}
		}
	}

	void Print() const {
		for (int row = 0; row < height_; row++) {
			for (int col = 0; col < width_; col++) {
				std::cout<< std::setw(4) << data_[row][col] <<" ";
			}
			std::cout<<std::endl;
		}
	}

	const std::vector<T>& operator[](const int row) const {
		if (row < 0 && row >= height_) {
			throw std::string("Row outside range");
		}
		return data_[row];
	}


	std::vector<T>& operator[](const int row) {
		if (row < 0 && row >= height_) {
			throw std::string("Row outside range");
		}
		return data_[row];
	}

	Matrix operator*=(const T scalar) {
		for (int row = 0; row < height_; row++) {
			for (int col = 0; col < width_; col++) {
				data_[row][col] *= scalar;
			}
		}
		return *this;
	}

	Matrix operator+=(const T scalar) {
		for (int row = 0; row < height_; row++) {
			for (int col = 0; col < width_; col++) {
				data_[row][col] += scalar;
			}
		}
		return *this;
	}

	Matrix operator-=(const T scalar) {
		for (int row = 0; row < height_; row++) {
			for (int col = 0; col < width_; col++) {
				data_[row][col] -= scalar;
			}
		}
		return *this;
	}

	Matrix operator*(const T scalar) const {
		Matrix product(height_, width_);
		for (int row = 0; row < height_; row++) {
			for (int col = 0; col < width_; col++) {
				product[row][col] = data_[row][col] * scalar;
			}
		}
		return product;
	}

	Matrix operator*(const Matrix& other) const {
		if (other.Height() != width_) {
			throw std::string("Matrix dimension mismatch for multiplcation");
		}
		Matrix product(height_, other.Width());
		for (int row = 0; row < height_; row++) {
			for (int col = 0; col < other.Width(); col++) {
				T sum = 0;
				for (int index = 0; index < width_; index++) {
					sum += data_[row][index] * other[index][col];
				}
				product.At(row, col) = sum;
			}
		}
		return product;
	}

	Matrix operator+=(const Matrix& other) {
		if (other.Height() != height_ || other.Width() != width_) {
			throw std::string("Matrix dimension mismatch for sum.");
		}
		for (int row = 0; row < height_; row++) {
			for (int col = 0; col < other.Width(); col++) {
				data_[row][col] += other[row][col];
			}
		}
		return *this;
	}

	Matrix operator+(const Matrix& other) const {
		if (other.Height() != height_ || other.Width() != width_) {
			throw std::string("Matrix dimension mismatch for sum.");
		}
		Matrix sum(height_, other.Width());
		for (int row = 0; row < height_; row++) {
			for (int col = 0; col < other.Width(); col++) {
				sum[row][col] = data_[row][col] + other[row][col];
			}
		}
		return sum;
	}

	Matrix operator-(const Matrix& other) const {
		if (other.Height() != height_ || other.Width() != width_) {
			throw std::string("Matrix dimension mismatch for sum.");
		}
		Matrix subtraction(height_, other.Width());
		for (int row = 0; row < height_; row++) {
			for (int col = 0; col < other.Width(); col++) {
				subtraction[row][col] = 
					data_[row][col] - other[row][col];
			}
		}
		return subtraction;
	}

	Matrix operator-=(const Matrix& other) {
		if (other.Height() != height_ || other.Width() != width_) {
			throw std::string("Matrix dimension mismatch for sum.");
		}
		for (int row = 0; row < height_; row++) {
			for (int col = 0; col < other.Width(); col++) {
				data_[row][col] -= other[row][col];
			}
		}
		return *this;
	}

	T Determinant() {
		const auto& rank_computation = Rank();
		int rank = rank_computation.first; 
		if (rank < std::min(height_, width_)) {
			return 0;
		}
		T prod = 1;
		for (int row = 0; row < rank; row++) {
			prod *= data_[row][row];
		}
		int num_row_swaps = rank_computation.second;
		return prod * (num_row_swaps % 2 == 0 ? 1 : -1);
	}

	std::pair<int, int> Rank() {
		int num_row_swaps = GaussElimination();
		int row = 0;
		for (; 
			(row < std::min(width_, height_)) 
			&& (data_[row][row] != 0); 
			row++) {}
		return std::make_pair(row, num_row_swaps);
	}

	Matrix(const Matrix&) = default;
	Matrix& operator=(const Matrix&) = default;
	Matrix(Matrix&&) = default;
	Matrix& operator=(Matrix&&) = default;

private:

	int GaussElimination() {
		int num_row_swaps = 0;
		for (int row = 0; row < height_; row++) {
			int col = row;
			int nonzero_row_index = FindNonZeroColumnEntry(
				row, height_ - 1, col);
			if (nonzero_row_index >= height_) {
				int nonzero_col_index = FindNonZeroRowEntry(
					col, width_ - 1, row);
				if (nonzero_col_index >= width_) {
					continue;	
				}
				col = nonzero_col_index;
			} else {
				if (nonzero_row_index != row) {
					SwapRows(row, nonzero_row_index);
					num_row_swaps++;
				}	
			}		
			for (int row2 = row + 1; row2 < height_; row2++) {
				SubtractRow(row, row2, col);
			}

			for (int row2 = row + 1; row2 < height_; row2++) {
				bool is_all_zero = FindNonZeroRowEntry(
					0, width_ - 1, row2) >= width_;
				if (is_all_zero) {
					int nonzero_row_index = FindNonZeroColumnEntry(
						row2, height_ - 1, 
						std::min(row2, width_ - 1));
					if (nonzero_row_index < height_) {
						SwapRows(row2, nonzero_row_index);
						num_row_swaps++;
					}
				}
			}
		}
		return num_row_swaps;
	}

	int FindNonZeroRowEntry(
		int start_col, 
		int end_col, 
		int row_of_interest) {
		if (start_col < 0 || start_col >= width_) {
			throw std::string("start_row exceeds matrix width");
		}
		if (end_col < 0 || end_col >= width_) {
			throw std::string("end_col exceeds matrix width");
		}
		if (row_of_interest < 0 || row_of_interest >= height_) {
			throw std::string("row_of_interest exceeds matrix height");
		}
		int col = width_;
		for (col = start_col; col <= end_col; col++) {
			if (fabs(
					static_cast<float>(
						data_[row_of_interest][col])) 
				> kMinMatrixValue) {
				return col;
			}
		}
		return col;
	}

	int FindNonZeroColumnEntry(
		int start_row, 
		int end_row, 
		int col_of_interest) {
		if (start_row < 0 || start_row >= height_) {
			throw std::string("start_row exceeds matrix height");
		}
		if (end_row < 0 || end_row >= height_) {
			throw std::string("end_row exceeds matrix height");
		}
		if (col_of_interest < 0 || col_of_interest >= width_) {
			throw std::string("col_of_interest exceeds matrix width");
		}
		int row = height_;
		for (row = start_row; row <= end_row; row++) {
			if (fabs(
					static_cast<float>(
						data_[row][col_of_interest])) > kMinMatrixValue) {
				return row;
			}
		}
		return row;
	}

	void SubtractRow(int row1, int row2, int col_of_interest) {
		if (row1 < 0 || row1 >= height_) {
			throw std::string("row1 exceeds matrix height");
		}
		if (row2 < 0 || row2 >= height_) {
			throw std::string("row2 exceeds matrix height");
		}
		if (col_of_interest < 0 || col_of_interest >= width_) {
			throw std::string("col exceeds matrix width");
		}

		if (data_[row1][col_of_interest] == 0 || data_[row1][col_of_interest] == 0) {
			return;
		}

		float ratio = static_cast<float>(data_[row2][col_of_interest]) / static_cast<float>(data_[row1][col_of_interest]);
		for (int col = 0; col < width_; col++) {
			data_[row2][col] = static_cast<T>(
				data_[row2][col] - ratio * data_[row1][col]);
		}
	}


	void SwapRows(const int row1, const int row2) {
		if (row1 < 0 || row1 >= height_) {
			throw std::string("row1 exceeds matrix height");
		}
		if (row2 < 0 || row2 >= height_) {
			throw std::string("row2 exceeds matrix height");
		}
		auto original_row1 = std::move(data_[row1]);
		data_[row1] = std::move(data_[row2]);
		data_[row2] = original_row1;
	}

	int width_;
	int height_;
	std::vector<std::vector<T>> data_;
};
}  // namespace

int main(int argc, char** argv) {
	if (false) {
		Matrix<int> m1(3,3);
		m1.RandInit(0, 3);
		m1 *= 2;
		std::cout<<"Printing m1"<<std::endl;
		m1.Print();
		Matrix<int> m2(3,3);
		m2.RandInit(0, 3);
		std::cout<<"Printing m2"<<std::endl;
		m2.Print();
		const auto& m3 = m1 + m2;
		std::cout<<"Printing m3 = m1 + m2"<<std::endl;
		m3.Print();
	
		std::cout<<"Printing m1 += m2"<<std::endl;
		(m1 += m2).Print();
		const auto& m4 = m1*m2;
		std::cout<<"Printing m4 = m1 * m2"<<std::endl;
		m4.Print();
	} else {
		Matrix<float> m1(3,3);
		m1.RandInit(0, 3);
		std::cout<<"Printing m1"<<std::endl;
		m1.Print();
		const auto& rank_computation = m1.Rank();
		int rank = rank_computation.first;
		std::cout<<"Reducing m1 to Reduced Row Echelon form with Gauss Elimination"<<std::endl;
		std::cout<<"Printing m1"<<std::endl;
		m1.Print();
		std::cout<<"Rank of m1 "<<rank<<std::endl;
		std::cout<<"Determinant of m1 "<<m1.Determinant()<<std::endl;
	}

	return 0;
}