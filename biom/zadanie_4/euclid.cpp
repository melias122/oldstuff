#include "euclid.h"

double Euclid::classify()
{
	int predictions = 0;
	int total = 0;
	for (int i = 0; i < data->test_projections.size(); i++) {
		for (const auto &projection : data->test_projections[i]) {
			total++;
			if (predict(projection) == i) {
				predictions++;
			}
		}
	}

	return double(predictions * 100) / total;
}

int Euclid::predict(const Mat &projection)
{
	double min = DBL_MAX;
	int label = 0;

	for (int i = 0; i < data->train_projections.size(); i++) {
		double dist = norm(data->train_projections[i], projection, NORM_L2);
		if (dist > min)
			continue;

		min = dist;
		label = data->labels[i];
	}

	return label;
}
