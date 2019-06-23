#include <iostream>
#include <algorithm>
#include <vector>

namespace {
class Event {
public:
  Event (const int start, const int end, const int interest) {
    this->start = start;
    this->end = end;
    this->interest = interest;
  }
  int start;
  int end;
  int interest;
};

void Display(const std::vector<Event> events) {
  for (const auto& e : events) {
    std::cout<<"("<<e.start<<", "<<e.end<<", "<<e.interest<<") ";
  }
  std::cout<<std::endl;
}

std::vector<Event> RightJoin(
  const std::vector<Event>& set_a, 
  const std::vector<Event>& set_b) {
    std::vector<Event> only_in_b;
    if (set_a.empty()) {
      return only_in_b;
    }
    for (const auto& b: set_b) {
      if (b.start >= set_a.back().end) {
        only_in_b.push_back(b);
      }
    }
    return only_in_b;
  }
  
std::vector<Event> LeftJoin(
  const std::vector<Event>& set_a, 
  const std::vector<Event>& set_b) {
    return RightJoin(set_b, set_a);
  }  
  
std::vector<Event> InnerJoin(
  const std::vector<Event>& set_a, 
  const std::vector<Event>& set_b) {

    std::vector<Event> in_a_b;
    if (set_a.empty()) {
      return in_a_b;
    }  
   for (const auto& b: set_b) {
      if (b.start < set_a.back().end) {
        in_a_b.push_back(b);
      }
    }

    return in_a_b;
  }

int FindMax(
  const std::vector<Event>& sorted_events) {
  if (sorted_events.size() == 0) {
    return 0;
  }
  if (sorted_events.size() == 1) {
    return sorted_events.front().interest;
  }

  const auto& a = sorted_events[0];

  std::vector<Event> all_but_a = 
  (sorted_events.size() > 1) 
  ? std::vector<Event>(sorted_events.begin() + 1, sorted_events.end()) 
  : std::vector<Event>();

  const std::vector<Event>& touches_a = 
  InnerJoin({a}, all_but_a);

  const std::vector<Event>& nottouches_a =
  RightJoin({a}, all_but_a);   

  const auto& b = touches_a;
  const std::vector<Event>& nottouches_b = 
    RightJoin(b, all_but_a);

  const std::vector<Event>& common =
  InnerJoin(nottouches_a, nottouches_b);

  const std::vector<Event>& only_in_nottouches_a = 
  LeftJoin(nottouches_a, nottouches_b);
  
  const std::vector<Event>& only_in_nottouches_b = 
  RightJoin(nottouches_a, nottouches_b);

  int common_cost = FindMax(common);
  int cost_with_first = 
  a.interest + FindMax(only_in_nottouches_a) + common_cost;
  
  int cost_without_first = FindMax(b) + FindMax(only_in_nottouches_b) + common_cost ;
  return std::max(cost_with_first, cost_without_first) ;
}


} // namespace  

int main(int argc, char **argv) {
  std::cout << "Hello, World! \n";
  std::vector<Event> events = {
    Event(3, 4, 4), 
    Event(1, 2, 3), 
    Event(0, 3, 10),
    Event(3, 5, 1),
    Event(6, 10, 12),
    Event(4, 7, 2),
  };
  std::cout<<"events ";Display(events);
  std::sort(events.begin(), events.end(), 
    [](const Event& a, const Event& b) {
      if (a.end != b.end) {
        return a.end < b.end;
      } else {
        return a.start < b.start;
      }
    });
  const std::vector<Event>& sorted_events = events;
  std::cout<<"sorted_events";Display(sorted_events);
  int max_interest = FindMax(sorted_events);
  std::cout<<"max_interest "<<max_interest<<std::endl;
  return 0;
}