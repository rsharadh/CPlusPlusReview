#include <algorithm>
#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>

namespace {

struct Station {
	Station(const std::string& name, int local_cost) 
	: name(name), prev(nullptr), 
	local_cost(local_cost), time_to_get_here(1e6), 
	prev_in_line(nullptr), prev_out_of_line(nullptr), is_optimized(false) {}
	std::string name;
	int local_cost;
	Station* prev_in_line;
	int in_line_transfer_cost;
	Station* prev_out_of_line;
	int switching_cost;

	// Computed properties
	Station* prev;
	int time_to_get_here;
	bool is_optimized;
};

void SetInLineNeighbors(
	Station* a, 
	Station* b, 
	int in_line_transfer_cost) {
	a->prev_in_line = b;
	a->in_line_transfer_cost = in_line_transfer_cost;
}

void SetInLineNeighbors(
	const std::string& a, 
	const std::string& b, 
	int in_line_transfer_cost,
    std::unordered_map<std::string, Station*>& 
	name_to_station_map) {
	return SetInLineNeighbors(
		name_to_station_map[a], 
		name_to_station_map[b], 
		in_line_transfer_cost);	
}

void SetOutOfLineNeighbors(
	Station* a, 
	Station* b, 
	int switching_cost) {
	a->prev_out_of_line = b;
	a->switching_cost = switching_cost;
}

void SetOutOfLineNeighbors(
	const std::string& a, 
	const std::string& b, 
	int switching_cost, 
	std::unordered_map<std::string, Station*>& name_to_station_map) {
	return SetOutOfLineNeighbors(
		name_to_station_map[a], 
		name_to_station_map[b], 
		switching_cost);	
}

void FindTimeToFinish(Station* destination) {
   if (destination->prev_in_line == nullptr) {
   	 destination->time_to_get_here = destination->local_cost;
   	 return;
   }
   if (destination->prev_out_of_line == nullptr) {
   	 destination->time_to_get_here = destination->local_cost;
   	 return;
   }		
   if (destination->prev_in_line->is_optimized == false) {
   		FindTimeToFinish(destination->prev_in_line);
   }
   if (destination->prev_out_of_line->is_optimized == false) {
   		FindTimeToFinish(destination->prev_out_of_line);
   }
   int time_to_in_line_prev = 
   	  destination->prev_in_line->time_to_get_here + 
   	  destination->in_line_transfer_cost;
   int time_to_out_of_line_prev = 
   	  destination->prev_out_of_line->time_to_get_here + 
   	  destination->switching_cost;

   if (time_to_in_line_prev < time_to_out_of_line_prev)	{
   	  destination->prev = destination->prev_in_line;
   }  else {
   	  destination->prev = destination->prev_out_of_line;
   }

   destination->time_to_get_here = 
   	std::min(time_to_in_line_prev, time_to_out_of_line_prev) 
   	+ destination->local_cost;
   destination->is_optimized = true;	
}

std::string PrintPath(Station* station) {
	std::string path_string_prev;
	if (station == nullptr) {
		return "";
	} else {
		path_string_prev += PrintPath(station->prev);
	}

	if (!path_string_prev.empty()) {
		 path_string_prev += " -> ";
	}

	return path_string_prev + station->name;
}

}  // namespace

int main(int argc, char** argv) {
	std::cout << "Hello World"<<std::endl;

	std::unordered_map<std::string, Station*> name_to_station_map ={
		{"A", new Station("A", 3)},
    	{"B", new Station("B", 4)},
    	{"C", new Station("C", 1)},
    	{"D", new Station("D", 6)},
    	{"E", new Station("E", 5)},
    	{"Z", new Station("Z", 4)},
    	{"Y", new Station("Y", 2)},
    	{"X", new Station("X", 4)},
    	{"W", new Station("W", 5)},
    	{"V", new Station("V", 1)},
    	{"start", new Station("start", 1)},
    	{"end", new Station("end", 1)},
	};

    std::vector<std::string> line_one = {"A", "B", "C", "D", "E"};
    std::vector<std::string> line_two = {"Z", "Y", "X", "W", "V"};


    SetInLineNeighbors("end", "A" , 1, name_to_station_map);
    SetInLineNeighbors("A", "B" , 1, name_to_station_map);
    SetInLineNeighbors("B", "C" , 1, name_to_station_map);
    SetInLineNeighbors("C", "D" , 1, name_to_station_map);
    SetInLineNeighbors("D", "E" , 1, name_to_station_map);
    SetInLineNeighbors("E", "start" , 1, name_to_station_map);
    name_to_station_map["E"]->prev = name_to_station_map["start"];

    SetOutOfLineNeighbors("A", "Y" , 2, name_to_station_map);
    SetOutOfLineNeighbors("B", "X" , 3, name_to_station_map);
    SetOutOfLineNeighbors("C", "W" , 4, name_to_station_map);
    SetOutOfLineNeighbors("D", "V" , 2, name_to_station_map);


    SetInLineNeighbors("Z", "Y" , 1, name_to_station_map);
    SetInLineNeighbors("Y", "X" , 1, name_to_station_map);
    SetInLineNeighbors("X", "W" , 1, name_to_station_map);
    SetInLineNeighbors("W", "V" , 1, name_to_station_map);
    SetInLineNeighbors("V", "start" , 1, name_to_station_map);
    name_to_station_map["V"]->prev = name_to_station_map["start"];

    SetOutOfLineNeighbors("end", "Z" , 2, name_to_station_map);
    SetOutOfLineNeighbors("Z", "B" , 3, name_to_station_map);
    SetOutOfLineNeighbors("Y", "C" , 2, name_to_station_map);
    SetOutOfLineNeighbors("X", "D" , 2, name_to_station_map);
    SetOutOfLineNeighbors("W", "E" , 3, name_to_station_map);

    FindTimeToFinish(name_to_station_map["end"]);
    std::cout<<PrintPath(name_to_station_map["end"])<<std::endl;
	return -1;
}