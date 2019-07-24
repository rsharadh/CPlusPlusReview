#include <iostream>
#include <chrono>
#include <random>
#include <vector>

namespace {
struct DataPoint {
	DataPoint() : confidence(0.f), is_positive(false) {}
	float confidence;
	bool is_positive;
};

struct OperatingPoint {
	OperatingPoint(const float threshold) : threshold(threshold), true_positives(0), true_negatives(0), false_positives(0), false_negatives(0) {}
	float threshold;
	int true_positives;
	int false_negatives;
	int true_negatives;
	int false_positives;
};

std::vector<OperatingPoint> GenerateOpPoints(int num_floats) {
	std::vector<OperatingPoint> op_points(num_floats, OperatingPoint(0.f));
	float delta = 1.f / (num_floats + 1);
	float sum = 0.f;
	for (auto& val : op_points) {
		sum += delta;
		val.threshold = sum;
	}
	return op_points;
}

std::vector<DataPoint> GenerateDataPoints(int num_floats) {
	unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
  	std::default_random_engine generator(seed);
	std::uniform_real_distribution<float> distribution(0, 1.f);

	std::vector<DataPoint> data(num_floats, DataPoint());
	for (auto& pt : data) {
		pt.confidence = distribution(generator);
		pt.is_positive = distribution(generator) > 0.5f;
		//std::cout<<"confidence "<<pt.confidence<<" "<<pt.is_positive<<std::endl;
	}
	return data;		
}

void NaivePRCurveComputation(
	const std::vector<DataPoint>& points,
	std::vector<OperatingPoint>* op_points) {
	if (op_points == nullptr) {
		throw std::string("nullptr for op_points");
	}

	for (const auto& pt: points) {
		for (auto& op: *op_points) {
			if (pt.is_positive) {
				if (pt.confidence > op.threshold) {
					op.true_positives++;
				} else {
					op.false_negatives++;
				}
			} else {
				if (pt.confidence > op.threshold) {
					op.false_positives++;
				} else {
					op.true_negatives++;
				}
			}	
		}
	}
}

void OptimizedPRCurveComputation(
	const std::vector<DataPoint>& points,
	int num_thresholds,
	std::vector<OperatingPoint>* op_points) {
	if (op_points == nullptr) {
		throw std::string("nullptr for op_points");
	}

	std::vector<int> positives_histogram(num_thresholds + 1, 0);
	std::vector<int> negatives_histogram(num_thresholds + 1, 0);
	//float delta = 1.f / (num_thresholds + 1);
	for (const auto& pt: points) {
		int bin = static_cast<int>(pt.confidence * (num_thresholds + 1));
		if (pt.is_positive) {
			positives_histogram[bin]++;
		} else {
			negatives_histogram[bin]++;
		}
	}

	int pos_sum = 0;
	for (auto& pos_bin : positives_histogram) {
		pos_sum += pos_bin;
		pos_bin = pos_sum;
	}

	int neg_sum = 0;
	for (auto& neg_bin : negatives_histogram) {
		neg_sum += neg_bin;
		neg_bin = neg_sum;
	}

	for (auto& op: *op_points) {
		int bin = -1 + static_cast<int>(op.threshold * (num_thresholds + 1));
		op.false_negatives = positives_histogram[bin];
		op.true_positives = pos_sum - positives_histogram[bin];
		op.false_positives = neg_sum - negatives_histogram[bin];
		op.true_negatives = negatives_histogram[bin]; 
	}
}

void PrintPRCurve(const std::vector<OperatingPoint> op_points) {
	for (const auto& op : op_points) {
		float precision = op.true_positives * 100.f / (op.false_positives + op.true_positives + 1e-3);
		float recall = op.true_positives * 100.f / (op.false_negatives + op.true_positives + 1e-3); 
		std::cout<<"threshold "<<op.threshold;
		std::cout<<" TP "<<op.true_positives<<" FN "<<op.false_negatives
			<<" FP "<<op.false_positives;
		std::cout<<" P "<<precision<<" R "<<recall<<std::endl;
	}
}
}  // namespace

int main(int argc, char** argv) {
	int num_points = 1000;
	int num_thresholds = 4;
	std::vector<DataPoint> points = GenerateDataPoints(num_points);
	{
		std::cout<<"NaivePRCurveComputation "<<std::endl;
		std::vector<OperatingPoint> op_points = GenerateOpPoints(num_thresholds);
		NaivePRCurveComputation(points, &op_points);
		PrintPRCurve(op_points);
	}
	{
		std::cout<<"OptimizedPRCurveComputation "<<std::endl;
		std::vector<OperatingPoint> op_points = GenerateOpPoints(num_thresholds);
		OptimizedPRCurveComputation(points, num_thresholds, &op_points);
		PrintPRCurve(op_points);	
	}
	return 0;
}