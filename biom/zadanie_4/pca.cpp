#include "pca.h"
#include <opencv2/highgui.hpp>

const vector<string> Pca::m_faces = {"butler", "obama", "putin", "schwarzenegger", "stallone", "willis"};

void Pca::init()
{
	m_trainImages.clear();
	m_testImages.clear();
	m_labels.clear();

	int label = 0;

	for (const auto &name : m_faces) {
		int train_size = m_trainSize, test_size = m_testSize;

		vector<Mat> test_data;

		for (int i = 1; i <= m_dataSize; i++) {
			string id_padded = to_string(i);
			id_padded = string(2 - id_padded.length(), '0') + id_padded;

			Mat image = imread("data/" + name + "_" + id_padded + ".bmp", 0);

			if ((train_size > 0 && rand() % 2) || test_size == 0) {
				m_trainImages.push_back(image);
				m_labels.push_back(label);
				train_size--;
			} else {
				test_data.push_back(image);
				test_size--;
			}
		}
		label++;
		m_testImages.push_back(test_data);
	}
}

void Pca::extract()
{
	Mat data = convVec(m_trainImages);
	int rows = data.rows;

	if (m_components <= 0 || m_components > rows)
		m_components = rows;

	PCA pca(data, Mat(), PCA::DATA_AS_ROW, int(m_components));

	m_data.clear();
	m_data.mean = pca.mean.clone().reshape(1, 1);
	m_data.eigenvalues = pca.eigenvalues.clone();
	transpose(pca.eigenvectors, m_data.eigenvectors);

	for (int i = 0; i < rows; i++) {
		Mat projected = pca.project(data.row(i).clone());
		m_data.train_projections.push_back(projected);
	}

	for (int i = 0; i < m_faces.size(); i++) {
		vector<Mat> projections;
		for (const auto &image : m_testImages[i]) {
			Mat projected = pca.project(convMat(image));
			projections.push_back(projected);
		}
		m_data.test_projections.push_back(projections);
	}

	m_data.labels = m_labels;
}

Mat Pca::convVec(const vector<Mat> &images)
{
	Mat dst(int(images.size()), images[0].rows * images[0].cols, CV_32FC1);

	for (int i = 0; i < images.size(); i++) {
		Mat image_row = images[i].clone().reshape(1, 1);
		Mat row_i = dst.row(i);
		image_row.convertTo(row_i, CV_32FC1, 1 / 255.);
	}

	return dst;
}

Mat Pca::convMat(const Mat &image)
{
	return convVec(vector<Mat>(1, image));
}
