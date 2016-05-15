#include "euclid.h"

#include <limits>

using namespace cv;

std::pair<int, double> Euclid::predict(const cv::Mat &projection)
{
	int predicted = 0;
	double min = std::numeric_limits<double>::max();

	for (int i = 0; i < data->train.size(); i++) {
		double dist = norm(data->train[i], projection, NORM_L2);
		dist = (dist-mean)/stdDev;
		if (dist < 0) {
			dist *= (-1);
		}
		if (dist < min) {
			min = dist;
			predicted = data->labels[i];
		}
	}

	return std::make_pair(predicted, min);
}

void Euclid::init()
{
	std::vector<int> values;
	for (int i = 0; i < data->train.size() - 1; i++) {
		Mat a = data->train[i];
		for(int j = i+1; j < data->train.size(); j++) {
			Mat b = data->train[j];
			values.push_back(norm(a, b, NORM_L2));
		}
	}
	std::vector<double> a, b;
	cv::meanStdDev(values, a, b);
	mean = a[0];
	stdDev = b[0];
}
