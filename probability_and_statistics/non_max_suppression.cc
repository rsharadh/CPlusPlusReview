#include <iostream>
#include <chrono>
#include <random>
#include <vector>
#include <queue>

namespace {
constexpr int kXMax = 100;
constexpr int kYMax = 100;

struct Point
{
	Point() : x(0), y(0) {}
	int x;
	int y;
};

struct PtWithConfidence : public Point {
	float confidence;
};

struct Box {
	Box() : confidence(0) {}
	Point tl;
	Point br;
	float confidence;
};

class BoxComparator {
public:
	bool operator()(const Box* lhs, const Box* rhs) {
		return lhs->confidence < rhs->confidence;
	}
};

std::vector<PtWithConfidence> GeneratePoints(int num_points) {
	unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
  	std::default_random_engine generator(seed);
	std::uniform_real_distribution<float> distribution(0, 1.f);

	std::vector<PtWithConfidence> data(num_points, PtWithConfidence());
	for (auto& pt : data) {
		pt.confidence = distribution(generator);
		pt.x = static_cast<int>(kXMax * distribution(generator));
		pt.y = static_cast<int>(kYMax * distribution(generator));
	}
	return data;		
}

std::vector<Box> GenerateBoxes(int num_boxes) {
	unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
  	std::default_random_engine generator(seed);
	std::uniform_real_distribution<float> distribution(0, 1.f);

	std::vector<Box> data(num_boxes, Box());
	for (auto& box : data) {
		box.confidence = distribution(generator);
		box.tl.x = static_cast<int>(kXMax * distribution(generator));
		box.tl.y = static_cast<int>(kYMax * distribution(generator));
		int box_width = static_cast<int>(kXMax / 10 * distribution(generator));
		int box_height = static_cast<int>(kYMax / 10 * distribution(generator));
		box.br.x = box.tl.x + box_width;
		box.br.y = box.tl.y + box_height;
	}
	return data;		
}

int Area(const Box& b) {
	return (b.br.x-b.tl.x + 1) * (b.br.y-b.tl.y + 1);
}

float IntersectLength(
	const std::pair<float, float>& interval_one,
	const std::pair<float, float>& interval_two) {
	if (interval_one.second < interval_one.first  
		|| interval_two.second < interval_two.first) {
		throw std::string("intervals incorrectly specified");
	}
	if (interval_one.second < interval_two.first 
		|| interval_one.first > interval_two.second) {
		return 0.f;
	}

	const auto& min_interval = 
		interval_one.first < interval_two.first 
		? interval_one 
		: interval_two;
	const auto& max_interval = 
		interval_one.first < interval_two.first 
		? interval_two 
		: interval_one;

	if (max_interval.second < min_interval.second) {
		if (max_interval.first < min_interval.first) {
			return (max_interval.second - min_interval.first) + 1;
		} else {
			return (max_interval.second - interval_two.first) + 1;
		}
	} else {
		return min_interval.second - max_interval.first + 1;
	}
}

float IoU(const Box& b1, const Box& b2) {
	float x_intersect = IntersectLength({b1.tl.x, b1.br.x}, {b2.tl.x, b2.br.x});
	float y_intersect = IntersectLength({b1.tl.y, b1.br.y}, {b2.tl.y, b2.br.y});
	float intersect_area = x_intersect * y_intersect;
	float b1_area = Area(b1);
	float b2_area = Area(b2);
	// std::cout<<"b1_area "<<b1_area<<std::endl;
	// std::cout<<"b2_area "<<b2_area<<std::endl;
	// std::cout<<"intersect_area "<<intersect_area<<std::endl;
	return intersect_area / (b1_area + b2_area - intersect_area);
}

std::vector<Box> NonMaxSuppress(
	const std::vector<Box>& input_boxes, 
	const float iou_thresh, 
	const float confidence_thresh) {
	std::vector<Box> nmsed_boxes;
	std::priority_queue<const Box*, std::vector<const Box*>, BoxComparator> box_q;
	for (const auto& box : input_boxes) {
		box_q.push(&box);
	}

	const Box* top_box = nullptr;
	while (!box_q.empty()) {
		if (top_box == nullptr) {
			top_box = box_q.top();
			box_q.pop();
			if (top_box->confidence >= confidence_thresh) {
				nmsed_boxes.push_back(*top_box);
				continue;
			} else {
				break;
			}
		}
		const Box* next_top_box = box_q.top();
		if (next_top_box->confidence < confidence_thresh) {
			break;
		}
		box_q.pop();
		// Need to optimize with some spatial partitioning.
		for (const auto& nmsed_box : nmsed_boxes) {
			if (IoU(nmsed_box, *next_top_box) > iou_thresh) {
				next_top_box = nullptr;
				break;
			}
		}
		if (next_top_box != nullptr) {
			nmsed_boxes.push_back(*next_top_box);
		}
	}
	return nmsed_boxes;
}

std::ostream& operator<<(std::ostream& os, const Point& pt) {
    os << pt.x << ", " << pt.y;
    return os;
}

std::ostream& operator<<(std::ostream& os, const Box& box) {
    os << box.confidence 
	   << " (" << box.tl << ") " 
	   <<"(" << box.br << ") "<<std::endl;
	return os;
}


std::ostream& operator<<(std::ostream& os, const std::vector<Box>& boxes) {
	for (const auto& box: boxes) {
    	os << box;
	}
	os << std::endl;
    return os;
}

}  // namespace

int main(int argc, char** argv) {
	Box b1;
	b1.tl.x=0; b1.tl.y=0;
	b1.br.x=9; b1.br.y=9;
	Box b2;
	b2.tl.x = 0; b2.tl.y = 0;
	b2.br.x = 4; b2.br.y = 4;
	std::cout<<b1<<" vs \n"<<b2<<"iou "<<IoU(b1, b2)<<std::endl;
	const auto& boxes = GenerateBoxes(5000);
	const auto& nmsed_boxes = 
		NonMaxSuppress(
			boxes, 
			/*iou_thresh=*/0.001f,
			/*confidence_thresh=*/0.5f);
	std::cout<<boxes.size()<<" Input Boxes "<<std::endl;
	//std::cout<<boxes<<std::endl;
	std::cout<<nmsed_boxes.size()<<" NMS'd boxes"<<std::endl;
	//std::cout<<nmsed_boxes<<std::endl;
	return 0;
}