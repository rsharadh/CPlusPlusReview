#include <iostream>
#include <vector>
#include <chrono>
#include <random>
#include <string>

namespace {
template<class T>
void Print(const std::vector<T> data) {
	for (const auto& elem : data) {
		std::cout<<elem<<" ";
	}
	std::cout<<std::endl;
}

template <class T>
class SequentialAccessor {
public:
	SequentialAccessor(const std::vector<T>& elements) : elements_(elements), index_(0) {}
	T GetNext() {
		T return_value = elements_[index_];
		index_ = (index_ + 1) % elements_.size();
		return return_value;
	}

	unsigned int Size() const {
		return elements_.size();
	}

private:
	int index_;
	const std::vector<T> elements_;
};

template <class T>
class ReservoirSampling {
public:
	static std::vector<T> SampleSequential(
		const unsigned int num_samples, SequentialAccessor<T>& s) {
		if (num_samples > s.Size()) {
			throw std::string("num_samples > SequentialAccessor::Size");
		}
		std::vector<T> bucket;
		bucket.reserve(num_samples);
		int sampling_index = 0;
		for (; sampling_index < num_samples; sampling_index++) {
			bucket.push_back(s.GetNext());
		}

		unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
  		std::default_random_engine generator(seed);

		while (sampling_index < s.Size()) {
			const auto current_val = s.GetNext();
			std::uniform_int_distribution<int> distribution(0, sampling_index);
			int selection_index = distribution(generator);
			if (selection_index < sampling_index) {
				bucket[selection_index] = current_val;
			}
			sampling_index++;
		}
		return bucket;
	}	
};

}  // namespace

int main(int argc, char** argv) {
	std::vector<std::string> str_vocabulary = {"A", "B", "C", "D", "E", "F", "G", "H", "I", "J"};
	std::cout<<"Vocabulary "<<std::endl;
	Print<std::string>(str_vocabulary);
	int num_samples = 5;
	SequentialAccessor<std::string> seq_accessor(str_vocabulary);
	
	auto bucket = ReservoirSampling<std::string>::SampleSequential(num_samples, seq_accessor);
	std::cout<<"Selected "<<num_samples<<" Bucket "<<std::endl;
	Print<std::string>(bucket);
	num_samples = str_vocabulary.size();
	bucket = ReservoirSampling<std::string>::SampleSequential(num_samples, seq_accessor);
	std::cout<<"Selected "<<num_samples<<" Bucket "<<std::endl;
	Print<std::string>(bucket);
	return 0;
}