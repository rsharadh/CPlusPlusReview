#include <iostream>
#include <chrono>
#include <random>

namespace {

template<class T>
T** Init(const int height, const int width) {
	T** data;
	if (height * width > 0) {
		data = new T*[height];
		data[0] = new T[height * width];
		for (unsigned int row_index = 1; 
			row_index < height; 
			row_index++) {
			data[row_index] = data[row_index - 1] +  width; 
		}

		for (unsigned int row = 0; row < height; row++) {
			for (unsigned int col = 0; col < width; col++) {
				data[row][col] = 0;
			}
		}
	} else {
		data = nullptr;
	}
	return data;
}		


template<class T>
class Image {
public:
	Image() : height_(0), width_(0), data_(nullptr) {}

	Image(const int height, const int width) : height_(height), width_(width) {
		data_ = Init<T>(height_, width_);
		sum_ =  Init<long>(height_, width_);
	}

	~Image() {
		if (data_ != nullptr) {
			delete[] data_;
		}
		if (sum_ != nullptr) {
			delete[] sum_;
		}
	}

	Image(const Image&) = delete;
	Image(Image&&) = delete;
	Image& operator=(const Image&) = delete;
	Image& operator=(Image&&) = delete;

	const T* operator[](const int row) const {
		if (row < height_ && row >= 0) {
			return data_[row];
		} else {
			throw std::string("Accesing invalid element");
		}
	}

	T* operator[](const int row) {
		if (row < height_ && row >= 0) {
			return data_[row];
		} else {
			throw std::string("Accesing invalid element");
		}
	}

	double Sum() const {
		double sum;
		for (unsigned int row = 0; row < height_; row++) {
			for (unsigned int col = 0; col < width_; col++) {
				sum += static_cast<double>(data_[row][col]);
			}
		}
		return sum;
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

	void Integrate() {
		sum_[0][0] = data_[0][0];
		unsigned int col = 0;
		for (unsigned int row = 1; row < height_; row++) {
			sum_[row][col] +=  sum_[row-1][col] + data_[row][col];
		}
		unsigned int row = 0;
		for (unsigned int col = 1; col < width_; col++) {
			sum_[row][col] += sum_[row][col-1] + data_[row][col];
		}
  		for (unsigned int row = 1; row < height_; row++) {
			for (unsigned int col = 1; col < width_; col++) {
				sum_[row][col] = 
					+ sum_[row - 1][col] 
					+ sum_[row][col - 1]
					- sum_[row - 1][col - 1] 
					+ data_[row][col]; 
			}
		}		
	}

	double Sum(int row_min, int col_min, int row_max, int col_max) {
		if (row_max < 0 || col_max < 0 || row_max >= height_ || col_max >= width_) {
			throw std::string("Out of bounds!!");
		}
		if (row_min > row_max || col_min > col_max) {
			throw std::string("Out of bounds!!");
		}
		double sum = 0;
		sum += static_cast<double>(sum_[row_max][col_max]);
		if (row_min >= 0 && col_min >= 0 && row_min < height_ && col_min < width_) {
				sum += static_cast<double>(sum_[row_min][col_min]);
		}
		if (row_min >=0 && row_min < height_) {
				sum += static_cast<double>(-sum_[row_min][col_max]);
		}
		if (col_min >=0 && col_min < width_) { 
			sum += static_cast<double>(-sum_[row_max][col_min]);
		}
		return sum; 
	}

	void Print() {
		for (unsigned int row = 0; row < height_; row++) {
			for (unsigned int col = 0; col < width_; col++) {
				std::cout<<static_cast<int>(data_[row][col])<<" "; 
			}
			std::cout<<std::endl;
		}	
	}

	void PrintSum() {
		for (unsigned int row = 0; row < height_; row++) {
			for (unsigned int col = 0; col < width_; col++) {
				std::cout<<sum_[row][col]<<" "; 
			}
			std::cout<<std::endl;
		}	
	}
private:
  int height_;
  int width_;
  T** data_;
  long** sum_;
};

}  // namespace

int main(int argc, char** argv) {

	// Image<unsigned char> uchar_image(100, 100);
	// std::cout<<"uchar_image sum "<<uchar_image.Sum()<<std::endl;
	// uchar_image.RandInit();
	// std::cout<<"uchar_image sum "<<uchar_image.Sum()<<std::endl;
	// uchar_image.Integrate();
	// std::cout<<"uchar_image sum "<<uchar_image.Sum(0, 0, 99, 99)<<std::endl;
	
	int num_rows = 5;
	int num_cols = 5;
	Image<unsigned char> int_image(num_rows, num_cols);
	std::cout<<"int_image sum "<<int_image.Sum()<<std::endl;
	int_image.RandInit(0, 1);
	std::cout<<"printing image after rand_init"<<std::endl;
	int_image.Print();
	std::cout<<"int_image[0, "<< num_cols-1 <<"] "
	<<int_image[0][num_cols-1]<<std::endl;
	std::cout<<"int_image["<<num_rows-1<< ", "
		<<num_cols-1<<"] "<<int_image[num_rows-1][num_cols-1]
		<<std::endl;
	std::cout<<"int_image sum "<<int_image.Sum()<<std::endl;
	std::cout<<"integrating image "<<std::endl;

	int_image.Integrate();
	std::cout<<"printing image sum after integrate"<<std::endl;
	int_image.PrintSum();
	std::cout<<"int_image sum (-1, -1, "<<num_rows-1<<", "
		<<num_cols-1<<") "
		<<int_image.Sum(-1, -1, num_rows-1, num_cols-1)
		<<std::endl;
	return 0;
}