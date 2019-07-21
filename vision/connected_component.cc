#include <iostream>
#include <chrono>
#include <random>
#include <deque>

#define IS_DEBUG 0

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


template <class T>	
void Print(T** data, const int height, const int width) {
	for (unsigned int row = 0; row < height; row++) {
		for (unsigned int col = 0; col < width; col++) {
			std::cout<<static_cast<int>(data[row][col])<<" "; 
		}
		std::cout<<std::endl;
	}	
}	


void Print(const std::vector<std::vector<int>>& label_equivalences,
	const std::string& display_string) {
#if IS_DEBUG	== 1
	std::cout<<display_string <<std::endl;
	for (int label_index = 0; 
		label_index < label_equivalences.size(); 
		label_index++) {
		for (auto& label: label_equivalences[label_index]) {
			std::cout<<label_index + 1<<" <=> "<<label<<std::endl;
		}
	}
#endif	
}

void Print(
	const std::vector<int>& label_equivalences_for,
	const int label_of_interest,
	const std::string& display_string) {
#if IS_DEBUG == 1
	std::cout<<display_string <<std::endl;
	for (auto label : label_equivalences_for) {
		std::cout<<label_of_interest<<" <=> "<<label<<std::endl;
	}
#endif
}

void Print(
	const std::vector<int>& label_equivalences_for,
	const int label_of_interest,
	const int equivalent_label,
	const std::string& display_string) {
#if IS_DEBUG == 1
	std::cout<<display_string <<std::endl;
	for (auto label : label_equivalences_for) {
		std::cout<<label_of_interest<<" <=> "
			<<label<<" == "
			<<equivalent_label<<std::endl;
	}
#endif	
}

void Print(const std::vector<int>& equivalent_labels,
	const std::string& display_string) {
#if IS_DEBUG == 1
	std::cout<<display_string <<std::endl;
	for (int label_index = 0; 
		label_index < equivalent_labels.size(); 
		label_index++) {
		std::cout<<label_index + 1<<" <=> "
			<<equivalent_labels[label_index]<<std::endl;
	}
#endif	
}

template<class T>
class Image {
public:
	Image() : height_(0), width_(0), data_(nullptr) {}

	Image(const int height, const int width) : height_(height), width_(width) {
		data_ = Init<T>(height_, width_);
		labels_ = Init<int>(height_, width_);
	}

	~Image() {
		if (data_ != nullptr) {
			delete[] data_;
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

	void RandInit(const T min = 0, const T max = 255) {
		unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
		//std::cout<<"seed "<<seed<<std::endl;
		//unsigned seed = 1892949966;
  		std::default_random_engine generator (seed);
  		for (unsigned int row = 0; row < height_; row++) {
			for (unsigned int col = 0; col < width_; col++) {
				std::uniform_int_distribution<T> distribution(min, max);
				data_[row][col] = distribution(generator);
			}
		}
	}

	void AddLabel(int* num_labels, 
		std::vector<std::vector<int>>* label_equivalences) {
		(*num_labels) += 1;
		label_equivalences->push_back(std::vector<int>());
	}

	bool IsLabelInEquivalences(
		const std::vector<int>& label_equivalences_for, 
		const int label_of_interest) {
		bool is_label_in_equivalences_list = false;
		for (const auto& label : label_equivalences_for) {
			if (label == label_of_interest) {
				is_label_in_equivalences_list = true;
				break;
			}
		}
		return is_label_in_equivalences_list;
	}

	void UpdateLabelEquivalences( 
		std::vector<std::vector<int>>* label_equivalences, 
		const int west_label, const int north_label) {
		if (west_label == north_label) {
			return;
		}
		if (!IsLabelInEquivalences(
				(*label_equivalences)[north_label-1], west_label)) {
			(*label_equivalences)[north_label-1].push_back(
				west_label);
		}

		if (!IsLabelInEquivalences(
				(*label_equivalences)[west_label-1], north_label)) {
			(*label_equivalences)[west_label-1].push_back(
				north_label);
		}
	}

	void FindConnectedComponents() {
		int num_labels = 0;
		std::vector<std::vector<int>> label_equivalences;
 		for (unsigned int row = 0; row < height_; row++) {
			for (unsigned int col = 0; col < width_; col++) {
				if (row == 0 && col == 0) {
					if (data_[row][col] != 0) {
						AddLabel(&num_labels, &label_equivalences);
						labels_[row][col] = num_labels;
					}
					continue;
				}
				T current_pixel = data_[row][col];
				if (current_pixel == 0) {
					continue;
				}
				if (row == 0) {
					T west_pixel = data_[row][col - 1];
					if (current_pixel == west_pixel) {
						labels_[row][col] = labels_[row][col - 1];
					} else {
						AddLabel(&num_labels, &label_equivalences);
						labels_[row][col] = num_labels;
					}
				}
				if (col == 0) {
					T north_pixel = data_[row - 1][col];
					if (current_pixel == north_pixel) {
						labels_[row][col] = labels_[row - 1][col];
					} else {
						AddLabel(&num_labels, &label_equivalences);
						labels_[row][col] = num_labels;
					}
				}
				if (row > 0 && col > 0) {
					T north_pixel = data_[row - 1][col];
					T west_pixel = data_[row][col - 1];
					int north_label = labels_[row - 1][col];
					int west_label = labels_[row][col - 1];
					if (north_pixel == 0 && west_pixel == 0) {
						AddLabel(&num_labels, &label_equivalences);
						labels_[row][col] = num_labels;
						continue;
					}
					if (north_pixel == 0) {
						labels_[row][col] = west_label;
						continue;
					}
					if (west_pixel == 0) {
						labels_[row][col] = north_label;
						continue;
					}
					UpdateLabelEquivalences(&label_equivalences, west_label, north_label);
					labels_[row][col] = west_label;
				}
			}
		}

		Print(label_equivalences, "Equivalent labels after first pass");
		std::vector<int> equivalent_labels(num_labels, 0);
		int really_unique_labels = 0;
		for (int label_index = 0; 
			label_index < label_equivalences.size(); 
			label_index++) {
			if (equivalent_labels[label_index] != 0) {
				continue;
			}
			really_unique_labels++;	
			equivalent_labels[label_index] = really_unique_labels;
 			if (label_equivalences[label_index].empty()) {
 				continue;
 			} 

			Print(
				label_equivalences[label_index], 
				label_index + 1,
				really_unique_labels, 
				"Equivalent labels outside queue");

			std::deque<int> label_queue;
			for (auto& label: label_equivalences[label_index]) {
				label_queue.push_back(label);
			}

			while (!label_queue.empty()) {
				int current_label = label_queue.front();
				label_queue.pop_front();
				if (equivalent_labels[current_label - 1] != 0) {
					continue;
				}
				equivalent_labels[current_label - 1] = 
					really_unique_labels;
				auto& label_equivalences_for_current_label = 
					label_equivalences[current_label - 1];	

				Print(
					label_equivalences_for_current_label, 
					current_label,
					"Equivalent labels inside queue");							
				for (
					auto& label : 
					label_equivalences_for_current_label) {							
					if (equivalent_labels[label - 1] == 0) {
						label_queue.push_back(label);
					}
				}
			}
		}
		Print(equivalent_labels, "Equivalent Labels identified");

 		for (unsigned int row = 0; row < height_; row++) {
			for (unsigned int col = 0; col < width_; col++) {
				int original_label = labels_[row][col];
				if (original_label == 0) {
					continue;
				}
				labels_[row][col] = equivalent_labels[original_label - 1];
			}
		}
	}

	T** Ptr() const {return data_;}
	int** LabelPtr() const {return labels_;}
	int Width() const {return width_;}
	int Height() const {return height_;}

private:
  int height_;
  int width_;
  T** data_;
  int** labels_;
};

}  // namespace

int main(int argc, char** argv) {
	int num_rows = 4;
	int num_cols = 4;
	Image<int> image(num_rows, num_cols);
	image.RandInit(0, 1);
	std::cout<<"printing image after rand_init"<<std::endl;
	Print<int>(image.Ptr(), image.Height(), image.Width());
	image.FindConnectedComponents();
	std::cout<<"printing image connected components"<<std::endl;
	Print<int>(image.LabelPtr(), image.Height(), image.Width());
	return 0;
}