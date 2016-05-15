#ifndef pca_h
#define pca_h

#include <vector>
#include <opencv2/core.hpp>

class Pca
{
public:
	struct Data {
		std::vector<cv::Mat> train;
		std::vector<std::vector<cv::Mat>> test;
		std::vector<int> labels;
	};

	Pca(const std::vector<cv::Mat> &train, const std::vector<std::vector<cv::Mat>> &test, const std::vector<int> &labels, int maxComponents = 50);
	Data* data() { return &m_data; }

	int predict(const cv::Mat &data);

private:
	Data m_data;
};

#endif
