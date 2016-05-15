#include "hamming.h"

#include <limits>

using cv::Mat;

void rotate(Mat &out, const Mat &in, int numRight)
{
	if (numRight == 0) {
		in.copyTo(out);
		return;
	}

	int ncols = in.cols;
	int nrows = in.rows;

	out = Mat::zeros(in.size(), in.type());

	numRight = numRight % ncols;
	if (numRight < 0)
		numRight = ncols + numRight;

	in(cv::Rect(ncols - numRight, 0, numRight, nrows)).copyTo(out(cv::Rect(0, 0, numRight, nrows)));
	in(cv::Rect(0, 0, ncols - numRight, nrows)).copyTo(out(cv::Rect(numRight, 0, ncols - numRight, nrows)));
}

// result = a | b
void logic_or(Mat &result, const Mat &a, const Mat &b)
{
	for (int r = 0; r < a.rows; r++) {
		for (int c = 0; c < a.cols; c++) {
			result.at<uchar>(r, c) = a.at<uchar>(r, c) | b.at<uchar>(r, c);
		}
	}
}

int hamming(const Mat &a, const Mat &b, const Mat &mask)
{
	int i = 0;

	for (int r = 0; r < a.rows; r++) {
		for (int c = 0; c < a.cols; c++) {
			if (mask.at<uchar>(r, c) == 0) {
				i += (a.at<uchar>(r, c) != b.at<uchar>(r, c));
			}
		}
	}

	return i;
}

// cmp compares a to b, rotate b -> returns min distance
int compare(const Iris &a, const Iris &b)
{
	Mat br, bmr;
	int min = (a.temp.rows * a.temp.cols) + 1;

	for (int i = -25; i < 25; i++) {

		// rotate b, mask b
		rotate(br, b.temp, i);
		rotate(bmr, b.mask, i);

		// create mask
		Mat mask(a.mask.size(), a.mask.type());
		logic_or(mask, a.mask, bmr);

		// compute hamming distance with mask
		int distance = hamming(a.temp, br, mask);
		if (distance < min) {
			min = distance;
		}
	}

	return min;
}

std::pair<int, double> Hamming::predict(const Iris &iris)
{
	int predicted = -1;
	double min = std::numeric_limits<double>::max();

	for (int i = 0; i < m_train.size(); i++) {

		double distance = compare(m_train[i], iris);
		distance = (distance - mean)/stdDev;
		if (distance < 0) {
			distance *= (-1);
		}
		if (distance < min) {
			min = distance;
			predicted = m_train[i].label;
		}
	}

	return std::make_pair(predicted, min);
}

void Hamming::init()
{
	std::vector<int> values;
	for (int i = 0; i < m_train.size() - 1; i++) {
		Iris a = m_train[i];
		for(int j = i+1; j < m_train.size(); j++) {
			Iris b = m_train[j];
			values.push_back(compare(a, b));
		}
	}
	std::vector<double> a, b;
	cv::meanStdDev(values, a, b);
	mean = a[0];
	stdDev = b[0];
}
