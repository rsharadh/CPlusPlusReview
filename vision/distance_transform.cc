#include <iostream>
#include <vector>
#include <chrono>
#include <random>

namespace {
template <class T>
void Print(const std::vector<std::vector<T>>& data) {
	for (const auto& row: data) {
		for (const auto& element : row) {
			std::cout<<element<<" ";
		}
		std::cout<<std::endl;
	}
}

class BinaryImage {
public:
	BinaryImage() : height_(0), width_(0) {}
	BinaryImage(const int height, const int width) : height_(height), width_(width) {
		data_ = std::vector<std::vector<bool>>(height_, std::vector<bool>(width_, 0));
		distance_ = std::vector<std::vector<int>>(height_, std::vector<int>(width_, INT_MAX/2));
	}

	void RandInit() {
		unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
  		std::default_random_engine generator (seed);
  		for (unsigned int row = 0; row < height_; row++) {
			for (unsigned int col = 0; col < width_; col++) {
				std::uniform_int_distribution<int> distribution(0, 10);
				data_[row][col] = (distribution(generator) >= 7);
				if (data_[row][col]) {
					distance_[row][col] = 0;
				}
			}
		}
	}


	void L1DistanceTransform() {
		// Traverse from top-left to bottom-right
		for (unsigned int row = 0; row < height_; row++) {
			for (unsigned int col = 0; col < width_; col++) {
				if (row == 0 && col == 0) {
					continue;
				}
				if (row == 0) {
					distance_[row][col] = 
						std::min(
							distance_[row][col], 
							distance_[row][col - 1] + 1);
					continue;	
				}
				if (col == 0) {
					distance_[row][col] = 
						std::min(
							distance_[row][col], 
							distance_[row - 1][col] + 1);
					continue;
				}
				distance_[row][col] = 
					std::min(
						distance_[row][col], 
						std::min(
							distance_[row - 1][col] + 1,
							distance_[row][col - 1] + 1));
				
			}
		}
		// Traverse from bottom-right to top-left
		for ( int row = height_ - 1; row >= 0; row--) {
			for ( int col = width_ - 1; col >= 0; col--) {
				if (row == height_ - 1 && col == width_ - 1) {
					continue;
				}

				if (row == height_ - 1) {
					distance_[row][col] = 
						std::min(
							distance_[row][col], 
							distance_[row][col + 1] + 1);
					continue;
				}


				if (col == width_ - 1) {
					distance_[row][col] = 
						std::min(
							distance_[row][col], 
							distance_[row + 1][col] + 1);
					continue;
				}

				distance_[row][col] = 
					std::min(
						distance_[row][col], 
						std::min(
							distance_[row + 1][col] + 1,
							distance_[row][col + 1] + 1));
			}
		}
	}


	void L2DistanceTransform() {
		throw std::string("L2DistanceTransform not supported yet");
	}	
	
	const std::vector<std::vector<int>>& Distance() const {
		return distance_;
	}


	const std::vector<std::vector<bool>>& Data() const {
		return data_;
	}
private:
	int height_;
	int width_;
	std::vector<std::vector<bool>> data_;
	std::vector<std::vector<int>> distance_;
};


}  // namespace

int main(int argc, char** argv) {
  BinaryImage b(5, 5);
  b.RandInit();
  std::cout<<"Data "<<std::endl;
  Print(b.Data());
  b.L1DistanceTransform();
  std::cout<<"\nL1DistanceTransform "<<std::endl;
  Print(b.Distance());
  return -1;
}
