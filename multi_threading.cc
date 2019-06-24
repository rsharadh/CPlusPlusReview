#include <iostream>
#include <vector>
#include <thread>
#include <chrono>
#include <string>

namespace {
struct Node {
	Node() : time(0) {}
	std::string name;
	int time;
};
void count_to_time(const std::vector<const Node*>& nodes, 
	const int node_index, int count_to_time) {
	 auto start = std::chrono::high_resolution_clock::now();

	 std::this_thread::sleep_for (std::chrono::seconds(count_to_time));
  
    // operation to be timed ...

    auto finish = std::chrono::high_resolution_clock::now();
    std::cout << std::chrono::duration_cast<std::chrono::nanoseconds>(finish-start).count() << "ns\n";
}

void count_to(const Node* node) {
	 auto start = std::chrono::high_resolution_clock::now();

	 std::this_thread::sleep_for (std::chrono::seconds(node->time));
  
    // operation to be timed ...

    auto finish = std::chrono::high_resolution_clock::now();
    std::cout << "node->name "<<node->name <<std::endl;
    std::cout << std::chrono::duration_cast<std::chrono::nanoseconds>(finish-start).count() << "ns\n";
}
class MyThreadh {
public:
	static void test(std::string command, int iters)
	{
		for(int i = 0; i < iters; i++)
		{
			std::cout<<command<<" :: "<<i<<std::endl;
		}
	}
 static void count_to_time(int count_to_time) {
		 auto start = std::chrono::high_resolution_clock::now();

		 std::this_thread::sleep_for (std::chrono::seconds(count_to_time));
	  
	    // operation to be timed ...

	    auto finish = std::chrono::high_resolution_clock::now();
	    std::cout << std::chrono::duration_cast<std::chrono::nanoseconds>(finish-start).count() << "ns\n";
	}
	static void count_to(Node* node) {
		 auto start = std::chrono::high_resolution_clock::now();

		 std::this_thread::sleep_for (
		 	std::chrono::seconds(node->time));

	    auto finish = std::chrono::high_resolution_clock::now();
	    std::cout << std::chrono::duration_cast<std::chrono::nanoseconds>(finish-start).count() << "ns\n";
	}
};

class Threadh {
public:

	static void count_to(int count_to_time) {
		 auto start = std::chrono::high_resolution_clock::now();

		 std::this_thread::sleep_for (std::chrono::seconds(count_to_time));
	  
	    // operation to be timed ...

	    auto finish = std::chrono::high_resolution_clock::now();
	    std::cout << std::chrono::duration_cast<std::chrono::nanoseconds>(finish-start).count() << "ns\n";
	}
};
}  // namespace


int main(int argc, char** argv) {

	//std::thread()
	Node* n = new Node();
	n->name = "First threadh";
	n->time = 2;
	//const auto node_vec = {n};
	//count_to_time(node_vec, 0, 2);
	
	std::thread p(count_to, n);
	p.join();
	std::thread r(&MyThreadh::count_to, n);
	std::thread s (&Threadh::count_to, 2);
	std::thread t(&MyThreadh::count_to_time, 2);
	r.join();
	s.join();
	t.join();
	// Create a thread using static member function
	std::thread th(&MyThreadh::test, "Task", 5);
 
	th.join();
	return 0;
}
