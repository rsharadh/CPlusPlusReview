#include <iostream>
#include <algorithm> 
#include <iomanip>
#include <chrono>
#include <random>
#include <vector>

namespace {
class Matrix {
public:
	Matrix() : width_(0), height_(0) {};
	Matrix(const int width, const int height) 
		: width_(width), height_(height) {
		for (int row = 0; row < height_; row++) {
			data_.push_back(std::vector<int>(width_, 0));
		}
	}

	int Height() const { 
		return height_;
	}

	int Width() const { 
		return width_;
	}

	int At(int row, int col) const {
		if (row < 0 && row >= height_) {
			throw std::string("Row outside range");
		}
		if (col < 0 && col >= width_) {
			throw std::string("Col outside range");
		}
		return data_[row][col];
	}


	int& At(int row, int col) {
		if (row < 0 && row >= height_) {
			throw std::string("Row outside range");
		}
		if (col < 0 && col >= width_) {
			throw std::string("Col outside range");
		}
		return data_[row][col];
	}

	void RandInit(const int min = 0, const int max = 255, const float float_thresh = 0.9f) {
		unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
  		std::default_random_engine generator (seed);
  		for (unsigned int row = 0; row < height_; row++) {
			for (unsigned int col = 0; col < width_; col++) {
				std::uniform_real_distribution<float> real_distribution(0,  1.f);
				if (real_distribution(generator) > float_thresh) {
					std::uniform_int_distribution<int> distribution(min, max);
					data_[row][col] = distribution(generator);
				}
			}
		}
	}

	void Print() const {
		std::cout<<"Matrix"<<std::endl;
		for (int row = 0; row < height_; row++) {
			for (int col = 0; col < width_; col++) {
				std::cout<< std::setw(4) << data_[row][col] <<" ";
			}
			std::cout<<std::endl;
		}
	}

	const std::vector<int>& operator[](const int row) const {
		if (row < 0 && row >= height_) {
			throw std::string("Row outside range");
		}
		return data_[row];
	}


	std::vector<int>& operator[](const int row) {
		if (row < 0 && row >= height_) {
			throw std::string("Row outside range");
		}
		return data_[row];
	}
private:
	int width_;
	int height_;
	std::vector<std::vector<int>> data_;

};

struct Entry {
	Entry() : row(0), col(0), value(0) {}
	Entry(const int row, const int col, const int val) : row(row), col(col), value(val) {}
	int row;
	int col;
	int value;
};

bool CompareEntries(const Entry& lhs, const Entry& rhs) {
	if (lhs.row != rhs.row) {
		return lhs.row < rhs.row;
	} else {
		return lhs.col < rhs.col;
	}
}

// Based on https://www.geeksforgeeks.org/operations-sparse-matrices/
class SparseMatrix {
public:
	SparseMatrix() : height_(0), width_(0) {}
	SparseMatrix(const Matrix& m) : height_(m.Height()), width_(m.Width()) {
		for (int row = 0; row < height_; row++) {
			for (int col = 0; col < width_; col++) {
				if (m[row][col] != 0) {
					entries_.push_back(Entry(row, col, m[row][col]));
				}
			}
		}

		std::sort(entries_.begin(), entries_.end(), CompareEntries);
	}
	int Height() const { 
		return height_;
	}

	int Width() const { 
		return width_;
	}

	void Print() {
		std::cout<<"Sparse Matrix"<<std::endl;
		for (const auto& entry : entries_) {
			std::cout<<entry.row<<" "<<entry.col<<" "<<entry.value<<std::endl;
		}
	}

	const std::vector<Entry>& Entries() const {
		return entries_;
	}

	Matrix ToMatrix() {
		Matrix m(height_, width_);
		for (const auto& entry : entries_) {
			m[entry.row][entry.col] = entry.value;
		}
		return m;
	}

	SparseMatrix& Transpose() {
		std::for_each(
			entries_.begin(), 
			entries_.end(), 
			[](Entry& entry) {
				int row = entry.row;
				int col = entry.col;
				entry.row = col;
				entry.col = row;
			});
		std::sort(entries_.begin(), entries_.end(), CompareEntries);
		return *this;
	};

private:
	int width_;
	int height_;
	std::vector<Entry> entries_;
};
}  // namespace

int main(int argc, char** argv) {
	Matrix m(4,4);
	m.RandInit(0, 5, 0.8);
	m.Print();
	SparseMatrix s(m);
	s.Print();
	s.Transpose().Print();
	s.ToMatrix().Print();
	return 0;
}