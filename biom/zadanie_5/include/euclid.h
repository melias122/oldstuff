#ifndef euclid_h
#define euclid_h

#include "pca.h"

class Euclid
{
	friend class Hybrid;
public:
	Euclid(const Pca::Data *data) : data(data)
	{
//		init();
	}
	std::pair<int, double> predict(const cv::Mat &projection);

private:
	const Pca::Data *data;
	double mean{0}, stdDev{1};

	void init();
};

#endif
