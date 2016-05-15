#include "svm.h"
#include <opencv2/ml.hpp>

double Svm::classify()
{
	if (data->train_projections.empty()) {
		return -1.;
	}

	int train_size = int(data->train_projections.size());
	int components = data->eigenvalues.rows;

	Mat train_data(train_size, components, CV_32FC1);
	Mat label_data(train_size, 1, CV_32SC1);

	for (int i = 0; i < train_size; i++) {
		data->train_projections[i].row(0).copyTo(train_data.row(i));
		label_data.at<int>(i, 0) = data->labels[i];
	}

	Ptr<ml::SVM> svm = ml::SVM::create();

	svm->setType(ml::SVM::C_SVC);
	svm->setGamma(0.0001);
	svm->setC(2000);
	svm->setKernel(ml::SVM::RBF);
	svm->train(train_data, ml::ROW_SAMPLE, label_data);

	int predictions = 0;
	int total = 0;

	for (int i = 0; i < data->test_projections.size(); i++) {
		for (const auto &projection : data->test_projections[i]) {
			total++;
			int prediction = svm->predict(projection);
			if (prediction == i)
				predictions++;
		}
	}

	return double(predictions * 100) / total;
}
