#include "hybrid.h"

#include <vector>

std::pair<int, double> Hybrid::predict(const cv::Mat &face, const Iris &iris)
{
	auto score_euclid  = euclid->predict(face);
	auto score_hamming = hamming->predict(iris);

	int label = 0;
	if (score_euclid.second < score_hamming.second) {
		label = score_euclid.first;
	} else {
		label = score_hamming.first;
	}

	return std::make_pair(label, score_euclid.second + score_hamming.second);
}
