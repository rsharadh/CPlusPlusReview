#include <iostream>
#include <vector>

int main(int argc, char** argv) {
	std::vector<int> line_1_transition_costs = 
	{1, 2, 3, 1, 4, 1};
	std::vector<int> line_2_transition_costs = 
	{1, 3, 1, 2, 3, 2};

	std::vector<int> line12_switching_costs = 
	{1, 3, 2, 1, 2,};
	std::vector<int> line21_switching_costs = 
	{2, 3, 1, 4, 1,};

	std::vector<int> line_1_arrived_from_at(line_1_transition_costs.size(), 0);
	std::vector<int> line_1_total(line_1_transition_costs.size(), 0);

	std::vector<int> line_2_arrived_from_at(line_2_transition_costs.size(), 0);
	std::vector<int> line_2_total(line_2_transition_costs.size(), 0);

	int start_cost = 10;
	line_1_total[0] = start_cost;
	line_2_total[0] = start_cost;

	for (int i = 1; i < line_1_transition_costs.size(); i++) {
		// best strategy to reach line_1[i]
		if (line_1_total[i-1] + line_1_transition_costs[i-1] <=
			line_2_total[i-1] + line21_switching_costs[i-1]) {
			line_1_arrived_from_at[i] = 1;
			line_1_total[i] = line_1_total[i-1] + line_1_transition_costs[i-1];
		} else {
			line_1_arrived_from_at[i] = 2;
			line_1_total[i] = line_2_total[i-1] + line21_switching_costs[i-1];
		}
		// best strategy to reach line 2.
		if (line_2_total[i-1] + line_2_transition_costs[i-1] <= 
			line_1_total[i-1] + line12_switching_costs[i-1]) {
			line_2_arrived_from_at[i] = 2;
			line_2_total[i] = line_2_total[i-1] + line_2_transition_costs[i-1];
		} else {
			line_2_arrived_from_at[i] = 1;
			line_2_total[i] = line_1_total[i-1] + line12_switching_costs[i-1];
		}

	}

	if (false) {
		std::cout<<std::endl;
		std::cout<<"line  1 totals \n";
		int step = 0;
		for (const auto tot: line_1_total) {
			std::cout<<tot<<" "<<line_1_transition_costs[step]<<" "<<line12_switching_costs[step]<<" ";		
			step++;
		}
		std::cout<<std::endl;
	
		std::cout<<"line  2 totals \n";
		step = 0;
		for (const auto tot: line_2_total) {
			std::cout<<tot<<" "<<line_2_transition_costs[step]<<" "<<line21_switching_costs[step]<<" ";		
			step++;		
		}
		std::cout<<std::endl;
	}

	int line_choise_at_finish;
	if (line_1_total.end() <= line_2_total.end()) {
		line_choise_at_finish = 1;
	} else {
		line_choise_at_finish = 2;
	}
	std::cout<<std::endl;
	int line_choise_at_i = line_choise_at_finish;
	for (int step = line_1_arrived_from_at.size(); step > 0; step--) {
			std::cout<<"arrived from  "
			<<line_choise_at_i<<" at time "
			<<step<<std::endl;
			line_choise_at_i = 
			line_choise_at_i == 1 
			? line_1_arrived_from_at[step-1] 
			: line_2_arrived_from_at[step-1];
	}
	return 0;
}
