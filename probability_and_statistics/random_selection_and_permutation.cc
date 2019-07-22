#include <iostream>
#include <vector>
#include <chrono>
#include <random>
#include <string>


namespace {

template <class T>
class SelectWithRepetition {
public:
	SelectWithRepetition(const std::vector<T> vocabulary) : vocabulary_(vocabulary){}
	virtual std::vector<T> GetK(unsigned int num_samples) {
		if (num_samples == 0) {
			return {};
		}
		unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
  		std::default_random_engine generator(seed);
  		std::vector<T> selections;
  		selections.reserve(num_samples);
  		while (num_samples > 0) {
			std::uniform_int_distribution<int> distribution(0, vocabulary_.size() - 1);
			selections.push_back(vocabulary_[distribution(generator)]);
			num_samples--;
  		}
		return selections;
	}
	
	// Disable copy/move constructor and copy/move assignment operators.	
	SelectWithRepetition(SelectWithRepetition&) = delete;
	SelectWithRepetition(SelectWithRepetition&&) = delete;
	SelectWithRepetition& operator=(const SelectWithRepetition&) = delete;
	SelectWithRepetition& operator=(SelectWithRepetition&&) = delete;
protected:
	const std::vector<T> vocabulary_;
};

template <class T>
class SelectWithoutRepetition : public SelectWithRepetition<T> {
public:
	SelectWithoutRepetition(const std::vector<T> vocabulary) : SelectWithRepetition<T>(vocabulary){}

	virtual std::vector<T> GetK(unsigned int num_samples) override {
		if (num_samples > this->vocabulary_.size()) {
			throw std::string("num_samples > vocabulary_.size()");
		}

		if (num_samples == 0) {
			return {};
		}
		unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
  		std::default_random_engine generator(seed);

		std::vector<T> selections;
  		selections.reserve(num_samples);
  		std::vector<T> sampling_vocabulary = this->vocabulary_;
  		int sampling_vocabulary_range = sampling_vocabulary.size() - 1;
  		while (num_samples > 0) {
			std::uniform_int_distribution<int> distribution(0, sampling_vocabulary_range);
			int selection_index = distribution(generator);
			selections.push_back(sampling_vocabulary[selection_index]);

			// Move sampled guy to the end
			T temp = std::move(sampling_vocabulary[selection_index]);
			sampling_vocabulary[selection_index] = sampling_vocabulary[sampling_vocabulary_range];
			sampling_vocabulary[sampling_vocabulary_range] = temp;
			sampling_vocabulary_range--;
			num_samples--;
  		}
		return selections;
	}

	// Disable copy/move constructor and copy/move assignment operators.	
	SelectWithoutRepetition(SelectWithoutRepetition&) = delete;
	SelectWithoutRepetition(SelectWithoutRepetition&&) = delete;
	SelectWithoutRepetition& operator=(const SelectWithoutRepetition&) = delete;
	SelectWithoutRepetition& operator=(SelectWithoutRepetition&&) = delete;
};

template <class T>
class RandomPermutation {
public:
	RandomPermutation(const std::vector<T> vocabulary) : vocabulary_(vocabulary){}
	
	std::vector<T> RandPerm() {
		unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
  		std::default_random_engine generator(seed);

  		int sampling_vocabulary_range = vocabulary_.size() - 1;
  		while (sampling_vocabulary_range >= 0) {
			std::uniform_int_distribution<int> distribution(0, sampling_vocabulary_range);
			int selection_index = distribution(generator);
			// Move sampled guy to the end
			T temp = std::move(vocabulary_[selection_index]);
			vocabulary_[selection_index] = vocabulary_[sampling_vocabulary_range];
			vocabulary_[sampling_vocabulary_range] = temp;
			sampling_vocabulary_range--;
  		}

		return vocabulary_;
	}

	std::vector<T> RandPerm(unsigned int num_samples) {
		if (num_samples > vocabulary_.size()) {
			throw std::string("num_samples > vocabulary_.size()");
		}

		if (num_samples == 0) {
			return {};
		}
		unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
  		std::default_random_engine generator(seed);

  		int sampling_vocabulary_range = vocabulary_.size() - 1;
  		while (num_samples > 0) {
			std::uniform_int_distribution<int> distribution(0, sampling_vocabulary_range);
			int selection_index = distribution(generator);
			// Move sampled guy to the end
			T temp = std::move(vocabulary_[selection_index]);
			vocabulary_[selection_index] = vocabulary_[sampling_vocabulary_range];
			vocabulary_[sampling_vocabulary_range] = temp;
			sampling_vocabulary_range--;
			num_samples--;
  		}

		return std::vector<T>(vocabulary_.begin() + sampling_vocabulary_range + 1, vocabulary_.end());
	}
private:
	std::vector<T> vocabulary_;
};

template<class T>
void Print(const std::vector<T> data) {
	for (const auto& elem : data) {
		std::cout<<elem<<" ";
	}
	std::cout<<std::endl;
}

}  // namespace

int main(int argc, char** argv) {
	std::vector<int> int_vocabulary = {1, 2, 3, 4, 5};
	SelectWithRepetition<int> int_select(int_vocabulary);
	std::cout<<"Vocabulary "<<std::endl;
	Print<int>(int_vocabulary);
	int num_samples = 3;
	std::cout<<"Random selection of "<<num_samples
		<<" with repetition "<<std::endl;
	for (int index = 0; index < 3; index++) {	
	    Print<int>(int_select.GetK(num_samples));
	}
	    
	SelectWithoutRepetition<int> int_select_no_rep(int_vocabulary);
	std::cout<<"Random selection of "<<num_samples
		<<" without repetition "<<std::endl;
	for (int index = 0; index < 3; index++) {	
	    Print<int>(int_select_no_rep.GetK(num_samples));
	}

	std::vector<std::string> str_vocabulary = {"A", "B", "C", "D", "E", "F", "G", "H", "I", "J"};
	SelectWithRepetition<std::string> str_select(str_vocabulary);
	std::cout<<"Vocabulary "<<std::endl;
	Print<std::string>(str_vocabulary);
	std::cout<<"Random selection of "<<num_samples
		<<" with repetition "<<std::endl;
	for (int index = 0; index < 3; index++) {	
		Print<std::string>(str_select.GetK(num_samples));
	}

	SelectWithoutRepetition<std::string> str_select_no_rep(str_vocabulary);
	std::cout<<"Random selection of "<<num_samples
		<<" without repetition "<<std::endl;
	for (int index = 0; index < 3; index++) {	
		Print<std::string>(str_select_no_rep.GetK(num_samples));
	}

	RandomPermutation<std::string> str_perm(str_vocabulary);
	std::cout<<"Random "<<str_vocabulary.size()<<" permutation "<<std::endl;
	for (int index = 0; index < 3; index++) {	
		Print<std::string>(str_perm.RandPerm());
	}
	std::cout<<"Random "<<num_samples<<" permutation "<<std::endl;
	for (int index = 0; index < 3; index++) {	
		Print<std::string>(str_perm.RandPerm(num_samples));
	}
	return 0;
}