#include <iostream>
#include <algorithm>
#include <utility>
#include <vector>
#include <math.h>

namespace {
constexpr float kFloatThresh = 1e-5;
constexpr float kPI = 3.14159265f;
constexpr float kPIBy4 = kPI / 4.f;

struct Polar {
	float rho;
	float theta;

	Polar() : rho(0.f), theta(0.f) {}
	Polar(const float t) : rho(1.f), theta(t) {}
	Polar(const float r, const float t) : rho(r), theta(t) {}
};

template <class T>
struct Point {
	T x;
	T y;

	Point() : x(0), y(0) {}
	Point(const T x, const T y) : x(x), y(y) {}
	Point(const Polar& p) {
		x = p.rho * cos(p.theta);
		y = p.rho * sin(p.theta);
	}
	Point(Polar& p) {
		x = p.rho * cos(p.theta);
		y = p.rho * sin(p.theta);
	}

	Point(const Point<T>&) = default;
	Point<T>& operator=(const Point<T>& other) = default;
	Point(Point<T>&&) = default;
	Point<T>& operator=(Point<T>&& other) = default;

	Point<T>& operator-=(const Point<T>& other) {
		x -= other.x;
		y -= other.y;
		return *this;
	}

	Point<T>& operator+=(const Point<T>& other) {
		x += other.x;
		y += other.y;
		return *this;
	}

	Point<T> operator+(const Point<T>& other) const {
		auto sum = *this;
		sum.x += other.x;
		sum.y += other.y;
		return sum;
	}

	Point<T> operator-(const Point<T>& other) const {
		auto diff = *this;
		diff.x -= other.x;
		diff.y -= other.y;
		return diff;
	}

	Point<T> operator*(const float scale) const {
		auto prod = *this;
		prod.x *= scale;
		prod.y *= scale;
		return prod;
	}

	Point<T> operator/(const float divider) const {
		auto divide = *this;
		divide.x /= (divider + kFloatThresh);
		divide.y /= (divider + kFloatThresh);
		return divide;
	}

	bool operator==(const Point<T>& other) const {
		return 
			fabs(static_cast<float>(x - other.x)) < kFloatThresh 
			&& fabs(static_cast<float>(y - other.y)) < kFloatThresh;
	}

	bool operator!=(const Point<T>& other) const {
		return 
			fabs(static_cast<float>(x - other.x)) > kFloatThresh 
			|| fabs(static_cast<float>(y - other.y)) > kFloatThresh;
	}
};

template <class T>
std::ostream& operator<<(std::ostream& os, const Point<T>& pt)
{
    os << pt.x << ", " << pt.y;
    return os;
}

template <class T>
std::ostream& operator<<(std::ostream& os, const std::vector<Point<T>>& pts) {
	for (const auto& pt: pts) {
    	os << "(" << pt << ") ";
	}
	os << std::endl;
    return os;
}


typedef Point<int> Point2;
typedef Point<float> Point2f;

template <class T>
T Cross(const Point<T>& p1, const Point<T>& p2) {
	return p1.x * p2.y - p2.x * p1.y;
}

// True if p2 - p0 is anti clockwise from p1 - p0.
template <class T>
bool IsAntiClockWiseFrom(const Point<T>& p0,const Point<T>& p1, const Point<T>& p2) {
	return (Cross(p1 - p0, p2 - p0)) >= 0;
}

// True if p2 is anti clockwise from p1 wrt origin.
struct IsAntiClockWiseFromOrigin {
 	template <class T>
 	bool operator()(const Point<T>& p0, const Point<T>& p1) {
 		return IsAntiClockWiseFrom(Point<T>(), p0, p1);
 	}
};

// True if po->p1->p2 is a left turn
template <class T>
bool IsLeftTurn(const Point<T>& p0,const Point<T>& p1, const Point<T>& p2) {
	return IsAntiClockWiseFrom(p0, p1, p2);
}

template <class T>
struct Stack {
	int Size() const {
		return vector_of_points.size();
	}

	bool Empty() const {
		return vector_of_points.empty();
	}
	void Push(const Point<T>& pt) {
		vector_of_points.push_back(pt);
	}

	Point<T> Pop() {
		if (Empty()) {
			throw std::string("Stack is empty!");
		}
		auto top = vector_of_points.back();
		vector_of_points.pop_back();
		return top;
	}

	const Point<T>& Top() const {
		if (Empty()) {
			throw std::string("Stack is empty!");
		}
		return vector_of_points.back();
	}
	const Point<T>& NextToTop() const {
		if (Size() < 2) {
			throw std::string("Stack has less than 2 elements!");
		}
		return vector_of_points[vector_of_points.size() - 2];
	}

	std::vector<Point<T>>& GetElements() {
		return vector_of_points;
	}
private:
	std::vector<Point<T>> vector_of_points;
};

// Finds convex hull by Graham Scan which is an O(NlogN) operation.
std::vector<Point2f> GrahamScan(std::vector<Point2f> pts) {
	auto ptr_to_bottom_left_most = std::min_element(
		pts.begin(), pts.end(), 
		[](const Point2f& a, const Point2f& b) {
			if (fabs(a.y - b.y) > kFloatThresh) {
				return a.y < b.y;	
			} else {
				return a.x < b.x;	
			}
		});
	Point2f p00 = *ptr_to_bottom_left_most;
	pts.erase(ptr_to_bottom_left_most, ptr_to_bottom_left_most + 1);
	std::sort(pts.begin(), pts.end(),
		[&p00](const Point2f& p0, const Point2f& p1) { 
			return IsAntiClockWiseFrom<float>(p00, p0, p1);
		});

	//std::cout<<pts<<std::endl;

	Stack<float> stack;
	stack.Push(p00);
	stack.Push(pts[0]);
	stack.Push(pts[1]);
	for (int i = 2; i < pts.size(); i++) {
		while (
			(stack.Size() > 1) 
			&& !IsLeftTurn(
				stack.NextToTop(), 
				stack.Top(), 
				pts[i])) {
			stack.Pop();
		}
		stack.Push(pts[i]);
	}
	std::cout<<"Convex hull by GrahamScan \n"<<stack.GetElements()<<std::endl;
	return stack.GetElements();
}

// Finds convex hull by Jarvis March, which is O(Nh) operation,
// for N points on a h point convex hull. If h << logN, 
// Jarvis March is more efficient than Graham scan.
std::vector<Point2f> JarvisMarch(std::vector<Point2f> pts) {
	auto ptr_to_bottom_left_most = std::min_element(
		pts.begin(), pts.end(), 
		[](const Point2f& a, const Point2f& b) {
			if (fabs(a.y - b.y) > kFloatThresh) {
				return a.y < b.y;	
			} else {
				return a.x < b.x;	
			}
		});
	Point2f p00 = *ptr_to_bottom_left_most;
	pts.erase(ptr_to_bottom_left_most, ptr_to_bottom_left_most + 1);

	Point2f current_pt = p00;
	Point2f next_pt;
	std::vector<Point2f> convex_hull;

	// Scan from bottom-left most point (in clockwise direction i.e. right turns)
	// till you hit highest point.
	while (true) {
		auto ptr_to_min_angle = std::min_element(
			pts.begin(), pts.end(), 
			[&current_pt](const Point2f& p0, const Point2f& p1) {
				return !IsLeftTurn<float>(
					current_pt, p0, p1);
			});
		next_pt = std::move(*ptr_to_min_angle);
		convex_hull.push_back(current_pt);
		if (current_pt.y > next_pt.y  - kFloatThresh) {
			break;
		}
		pts.erase(ptr_to_min_angle, ptr_to_min_angle + 1);
		current_pt = next_pt;
	}


	// Scan from bottom-left most point (in anti-clockwise direction i.e. left turns)
	// till you hit highest point.
	pts.push_back(convex_hull.back());
	convex_hull.pop_back();
	current_pt = p00;
	while (true) {
		auto ptr_to_min_angle = std::min_element(
			pts.begin(), pts.end(), 
			[&current_pt](const Point2f& p0, const Point2f& p1) {
				bool is_left_turn = IsLeftTurn<float>(
					current_pt, p0, p1);
				return is_left_turn;
			});
		next_pt = std::move(*ptr_to_min_angle);
		pts.erase(ptr_to_min_angle, ptr_to_min_angle + 1);
		if (current_pt != p00) {
			convex_hull.push_back(current_pt);
		}
		if (current_pt.y > next_pt.y - kFloatThresh) {
			break;
		}
		current_pt = next_pt;
	}
	std::cout<<"Convex hull by JarvisMarch "<<std::endl;
	std::cout<<convex_hull;

	return pts;
}
}  // namespace 

int main(int argc, char** argv) {
	if (false) {
		std::vector<Point2f> pts = {
			Polar(4*kPIBy4), Polar(5*kPIBy4), Polar(6*kPIBy4), 
			Polar(0.f), Polar(kPIBy4), Polar(2*kPIBy4), Polar(3*kPIBy4),
			Polar(7*kPIBy4)};
		std::cout<<"Points before sorting "<<std::endl;
		std::cout<<pts;	
		if (false) {
			std::sort(
				pts.begin(), 
				pts.end(), 
				IsAntiClockWiseFromOrigin());
		} else {
			std::sort(
				pts.begin(), pts.end(),
				[](const Point2f& p0, const Point2f& p1) {
					return IsAntiClockWiseFromOrigin()(p0, p1);
				});
		}
		std::cout<<"Points after sorting "<<std::endl;
		std::cout<<pts;

		auto p0 = Point2f(1, 0); 
		auto p1 = Point2f(0.707, 0.707); 
		auto p2 = Point2f(0.5, 0.5);
		if (IsLeftTurn<float>(p0, p1, p2)) {
			std::cout<<p0<<" -> "<<p1<<" -> "<<p2<<" forms a left turn. \n";
		} else {
			std::cout<<p0<<" -> "<<p1<<" -> "<<p2<<" forms a right turn. \n";
		}

	} else {
		std::vector<Point2f> pts = {
				Polar(0.f), 
				Polar(kPIBy4), 
				Polar(2*kPIBy4), 
				Polar(3*kPIBy4),
				Polar(4*kPIBy4), 
				Polar(5*kPIBy4), 
				Polar(6*kPIBy4), 
				Polar(7*kPIBy4)};
		
		pts.push_back((pts[0]+pts[3])/2);
		pts.push_back((pts[0]+pts[2])/2);
		pts.push_back((pts[1]+pts[3])/2);
		pts.push_back((pts[1]+pts[4])/2);
		pts.push_back((pts[2]+pts[4])/2);
		pts.push_back((pts[2]+pts[5])/2);
		pts.push_back((pts[3]+pts[5])/2);
		pts.push_back((pts[3]+pts[6])/2);
		pts.push_back((pts[4]+pts[6])/2);
		pts.push_back((pts[4]+pts[7])/2);
	
		Point2f offset(10.f, 10.f);
		for (auto& pt : pts) {
			pt += offset;
		}

		GrahamScan(pts);
		JarvisMarch(pts);
	}
	return 0;
}