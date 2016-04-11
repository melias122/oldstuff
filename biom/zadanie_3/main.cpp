#include <iostream>
#include <map>
#include "math.h"

#include "opencv2/opencv.hpp"

using namespace std;
using namespace cv;

struct k : public pair<int, int> {
	k(int i, int j)
		: pair(i, j)
	{
	}
};

struct Iris {
	Iris() {}
	explicit Iris(const Mat &t, const Mat &m)
		: temp{move(t)}
		, mask{move(m)}
	{
	}
	Mat temp, mask;
};

using Dataset = map<k, Iris>;

void showImage(const String &name, const Mat &image, bool wait = true)
{
	namedWindow(name, WINDOW_AUTOSIZE);
	imshow(name, image);
	if (wait)
		waitKey(0);
}

int readImage(Mat &image, const char *path)
{
	image = imread(path, 0);
	if (!image.data) {
		cout << "can't load " << path << endl;
		return 0;
	}
	return 1;
}

void Rot(Mat &out, Mat &in, int numRight)
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
void Or(Mat &result, const Mat &a, const Mat &b)
{
	for (int r = 0; r < a.rows; r++) {
		for (int c = 0; c < a.cols; c++) {
			result.at<uchar>(r, c) = a.at<uchar>(r, c) | b.at<uchar>(r, c);
		}
	}
}

int Ham(const Mat &a, const Mat &b, const Mat &mask)
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

struct CompareResult {
	int distance{0};
	int rotation{0};
	Mat a, b, mask;
};

// Compare a to b, rotate b
CompareResult Compare(const Iris &a, Iris &b, bool stats = false)
{
	Mat bRot, bMaskRot;
	int min = (a.temp.rows * a.temp.cols) + 1;
	CompareResult r;
	for (int i = -20; i < 20; i++) {

		// rotate b, bMask
		Rot(bRot, b.temp, i);
		Rot(bMaskRot, b.mask, i);

		// create mask
		Mat mask(a.mask.size(), a.mask.type());
		Or(mask, a.mask, bMaskRot);

		// compute hamming distance with mask
		int d = Ham(a.temp, bRot, mask);
		if (d < min) {
			min = d;
			r.rotation = i;
			r.distance = d;
		}
	}
	if (stats) {
		r.a = Mat::zeros(a.temp.size(), a.temp.type());
		r.b = Mat::zeros(b.temp.size(), b.temp.type());
		r.mask = Mat::zeros(a.mask.size(), a.mask.type());

		a.temp.copyTo(r.a);
		Rot(bRot, b.temp, r.rotation);
		bRot.copyTo(r.b);
		Rot(bMaskRot, b.mask, r.rotation);
		Or(r.mask, a.mask, bMaskRot);
	}
	return r;
}

Iris readIris(int person, int iris)
{
	Mat temp, mask;
	char path[100];
	sprintf(path, "iris/%d_%d_template.bmp", person, iris);
	if (!readImage(temp, path)) {
		printf("cant read %s\n", path);
		exit(1);
	}
	sprintf(path, "iris/%d_%d_mask.bmp", person, iris);
	if (!readImage(mask, path)) {
		printf("cant read %s\n", path);
		exit(1);
	}
	return Iris(temp, mask);
}

map<int, int> histogram(map<int, int> &m)
{
	map<int, int> hist;
	for (const auto &pair : m) {
		int tru = trunc(double(pair.first) / 100.0) * 100;
		int mod = pair.first % 100;

		if (mod < 50) {
			mod = 0;
		} else {
			mod = 50;
		}
		hist[tru + mod] += pair.second;
	}
	return hist;
}

void graph(int n = 5)
{
	Dataset data;
	// person
	for (int i = 1; i <= 10; i++) {
		for (int j = 1; j <= n; j++) {
			data[k(i, j)] = readIris(i, j);
		}
	}

	map<int, int> same, diff;
	for (int i = 1; i <= 10; i++) {

		// same iris
		for (int j = 1; j < n; j++) {
			const Iris &a = data[k(i, j)];
			for (int j2 = j + 1; j2 <= n; j2++) {
				Iris &b = data[k(i, j2)];
				int best = Compare(a, b).distance;
				same[best]++;
			}
		}

		// this person each iris
		for (int j = 1; j <= n; j++) {
			const Iris &a = data[k(i, j)];

			// other person
			for (int p = i + 1; p <= 10; p++) {

				// each iris
				for (int pi = 1; pi <= n; pi++) {
					Iris &b = data[k(p, pi)];
					int best = Compare(a, b).distance;
					diff[best]++;
				}
			}
		}
	}
	for (const auto &pair : histogram(same)) {
		cout << pair.first << " " << pair.second << endl;
	}
	cout << endl;
	for (const auto &pair : histogram(diff)) {
		cout << pair.first << " " << pair.second << endl;
	}
}

int main(int argc, char **argv)
{
	if (argc == 2) {
		char ch = 0;
		int n = 0;
		sscanf(argv[1], "%c%d", &ch, &n);
		if (ch == 'g' && n > 0) {
			graph(n);
			return 0;
		}
	}

	if (argc != 3) {
		printf("Usage: %s 1/1 2/1\n", argv[0]);
		exit(1);
	}

	int person, iris;

	sscanf(argv[1], "%d/%d", &person, &iris);
	Iris a = readIris(person, iris);

	sscanf(argv[2], "%d/%d", &person, &iris);
	Iris b = readIris(person, iris);

	CompareResult r = Compare(a, b, true);

	//	showImage("A mask", a.mask, false);
	//	showImage("B mask", b.mask, false);

	printf("Distance: %d\nRotation: %d\n", r.distance, r.rotation);
	showImage("A template", r.a, false);
	showImage("B template", r.b, false);
	showImage("A, B mask", r.mask);

	return 0;
}
