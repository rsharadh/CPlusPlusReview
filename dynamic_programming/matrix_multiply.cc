#include <iostream>
#include <vector>
#include <numeric>
#include <functional>
namespace {

int cumulative_product(const std::vector<int>& items) {
	return std::accumulate(items.begin(), items.end(), 1, std::multiplies<int>());
}

std::string GetSplitStringForMatrices(
	const std::vector<std::vector<int>>& split_location, 
	const int start_index, 
	const int end_index) {
	if (end_index >= split_location[0].size() || 
		start_index >= split_location.size()) {
		return "";
	} 
	if (start_index < 0 || end_index < 0) {
		return "";
	}
	int num_matrices = split_location.size();
	int num_matrices_considered = end_index - start_index + 1;
	std::string suffix = 
		(end_index == num_matrices-1 ? "" : "," );
	if (num_matrices_considered == 1) {
		return
		std::to_string(start_index) + suffix;
	}
	if (num_matrices_considered == 2) {
		return "(" + std::to_string(start_index) + ", " + 
			std::to_string(end_index) + ")" + suffix;
	}
	int first_split = split_location[start_index][end_index];
	return"(" + GetSplitStringForMatrices(split_location, start_index, first_split) 
		+ GetSplitStringForMatrices(split_location, first_split + 1, end_index) + ")" + suffix;
}
struct Matrix {
	Matrix(const int rows, const int cols) : rows(rows), cols(cols) {}
	int rows;
	int cols;
};

int calculate_multiplication_costs(const Matrix& a, const Matrix& b) {
	if (a.cols != b.rows) {
		std::cout<<"a dims "<<a.rows<<" "<<a.cols<<std::endl;
		std::cout<<"b dims "<<b.rows<<" "<<b.cols<<std::endl;
		throw std::string("incorrect matri dimensions crap");
	}
	return a.rows * a.cols * b.cols;
}

void OptimizeMatrixMultiplication(const std::vector<Matrix>& matrix_vector) {
	int num_matrices = matrix_vector.size();
	std::vector<std::vector<int>> multiplication_costs;
	std::vector<std::vector<int>> split_location;
	// Initialize.
	for (int index = 0; index < num_matrices ; index++) {
		multiplication_costs.push_back(std::vector<int>(num_matrices, 1e6));
		split_location.push_back(std::vector<int>(num_matrices, -1));
		multiplication_costs[index][index] = 0;
	}

	for (int start_index = 0; start_index < num_matrices - 1; start_index++) {
		multiplication_costs[start_index][start_index + 1] = 
			calculate_multiplication_costs(
				matrix_vector[start_index], 
				matrix_vector[start_index + 1]);
	}

	for (int offset = 2; offset < num_matrices; offset++) {
		for (int start_index = 0; start_index + offset < num_matrices; 
		start_index++) {
			int min_cost = 1e7;
			int min_split_location = -1;
			for (int running_offset = 0; 
				running_offset < offset; 
				running_offset++) {
				int candidate_split_loc = 
					start_index + running_offset;
				int candidate_cost = 
					multiplication_costs[
						start_index][
						candidate_split_loc]
					+ multiplication_costs[
						candidate_split_loc + 1][
						start_index + offset]
					+ matrix_vector[start_index].rows * 
					  matrix_vector[candidate_split_loc].cols * 
					  matrix_vector[start_index + offset].cols;
				if (candidate_cost < min_cost) {
					min_cost = candidate_cost;
					min_split_location = candidate_split_loc;
				}
			}
			multiplication_costs[start_index][start_index + offset] = min_cost;
			split_location[start_index][start_index + offset] = min_split_location;
		}
	}
	std::cout<<"Optimized cost "<<multiplication_costs[0][num_matrices-1]<<std::endl;
	std::cout<<GetSplitStringForMatrices(split_location, 0, num_matrices - 1)<<std::endl;
	// for (const auto& s1 : multiplication_costs) {
	// 	for (const auto& s2 : s1) {
	// 		std::cout<<s2<<" ";
	// 	}
	// 	std::cout<<std::endl;
	// }
	// for (const auto& s1 : split_location) {
	// 	for (const auto& s2 : s1) {
	// 		std::cout<<s2<<" ";
	// 	}
	// 	std::cout<<std::endl;
	// }	
}

}  // namespace

int main (int argc, char** argv) {
	// std::vector<int> matrix_mul_dims = 
	// {5,4,3,10,12,100,7,9,3,25,10,8,9};

	// std::vector<int> matrix_mul_dims = 
	// {5,4,3,10};
	std::vector<int> matrix_mul_dims = 
	{30, 35, 15, 5, 10, 20, 25};
	//OptimizeMatrixMultiplication(matrix_mul_dims);

	std::cout<<"Naieve calculation (sequential multiplies) "<<cumulative_product(matrix_mul_dims)<<std::endl;
	std::vector<Matrix> matrix_vector;
	for (int index = 0; index < matrix_mul_dims.size() - 1; ++index) {
		matrix_vector.push_back(
			Matrix(matrix_mul_dims[index], matrix_mul_dims[index + 1]));
	}
	OptimizeMatrixMultiplication(matrix_vector);
	return 0;
}