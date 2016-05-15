#ifndef hamming_h
#define hamming_h

#include <opencv2/core.hpp>

struct Iris {
	Iris(const cv::Mat &temp, const cv::Mat &mask) : temp{temp}, mask{mask} {}
	cv::Mat temp, mask;
	int label;
};

class Hamming
{
	friend class Hybrid;
public:
	Hamming(const std::vector<Iris> &train)
		: m_train{train}
	{
//		init();
	}
	std::pair<int, double> predict(const Iris &iris);

private:
	std::vector<Iris> m_train;

	void init();
	double mean{0}, stdDev{1};
};

#endif

