#ifndef hybrid_h
#define hybrid_h

#include "euclid.h"
#include "hamming.h"

class Hybrid
{
public:
	Hybrid(Euclid *euclid, Hamming *hamming) : euclid{euclid}, hamming{hamming} {}
	std::pair<int, double> predict(const cv::Mat &face, const Iris &iris);

private:
	Euclid *euclid;
	Hamming *hamming;
};

#endif

