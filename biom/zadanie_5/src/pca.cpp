#include "pca.h"

#include <opencv2/highgui.hpp>

using namespace std;
using namespace cv;

Mat convVec(const vector<Mat> &images)
{
	Mat dst(int(images.size()), images[0].rows * images[0].cols, CV_32FC1);

	for (int i = 0; i < images.size(); i++) {
		Mat image_row = images[i].clone().reshape(1, 1);
		Mat row_i = dst.row(i);
		image_row.convertTo(row_i, CV_32FC1, 1 / 255.);
	}

	return dst;
}

Mat convMat(const Mat &image)
{
	return convVec(vector<Mat>(1, image));
}


Pca::Pca(const vector<Mat> &train, const vector<vector<Mat> > &test, const vector<int> &labels, int maxComponents)
{
	Mat trainData = convVec(train);

	PCA pca(trainData, Mat(), PCA::DATA_AS_ROW, maxComponents);

	for (int i = 0; i < trainData.rows; i++) {
		Mat projected = pca.project(trainData.row(i));
		m_data.train.push_back(projected);
	}

	for (const vector<Mat> &faces : test) {
		vector<Mat> projections;
		for (const Mat &image : faces) {
			Mat projected = pca.project(convMat(image));
			projections.push_back(projected);
		}
		m_data.test.push_back(projections);
	}

	for (int label : labels) {
		m_data.labels.push_back(label);
	}
}
