#include "mahal.h"

void Mahal::makeProjections()
{
	m_classProjections.clear();
	m_meanProjections.clear();

	int classes = int(data->test_projections.size());
	int class_projections = int(data->train_projections.size()) / classes;
	int components = data->eigenvalues.rows;

	for (int i = 0; i < classes; i++) {
		m_classProjections.push_back(Mat(class_projections, components, CV_32FC1));
		m_meanProjections.push_back(Mat(1, components, CV_32FC1));
	}


	int class_counter = 0;
	int row_counter = 0;

	for (const auto &projection : data->train_projections) {
		projection.row(0).copyTo(m_classProjections[class_counter].row(row_counter));
		if ((row_counter + 1) % class_projections == 0) {
			reduce(m_classProjections[class_counter], m_meanProjections[class_counter], 0, CV_REDUCE_AVG);
			class_counter++;
			row_counter = 0;
		} else {
			row_counter++;
		}
	}
}

double Mahal::classify()
{
	if (data->train_projections.empty())
		return -1;
	makeProjections();

	int predictions = 0;
	int total_comparsions = 0;

	for (int i = 0; i < data->test_projections.size(); i++) {
		for (const auto &projection : data->test_projections[i]) {
			total_comparsions++;
			if (predict(projection) == i)
				predictions++;
		}
	}

	return (double(predictions * 100) / total_comparsions);
}

int Mahal::predict(const Mat &projection)
{
	double min = DBL_MAX;
	int label = 0;

	for (int i = 0; i < m_classProjections.size(); i++) {
		double dist = calculate(projection, i);
		if (dist > min)
			continue;

		min = dist;
		label = i;
	}

	return label;
}

double Mahal::calculate(const Mat &projection, const int class_id)
{
	Mat cov, mean;

	calcCovarMatrix(m_classProjections[class_id], cov, mean, CV_COVAR_ROWS | CV_COVAR_NORMAL);
	cov = cov / (m_classProjections[class_id].rows - 1);

	invert(cov, cov, DECOMP_SVD);

	cov.convertTo(cov, CV_32FC1);

	return Mahalanobis(projection, m_meanProjections[class_id], cov);
}
