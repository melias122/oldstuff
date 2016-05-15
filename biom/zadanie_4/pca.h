#ifndef pca_hpp
#define pca_hpp

#include <opencv2/core.hpp>
#include <vector>
#include <string>

using namespace cv;
using namespace std;

class Pca
{
public:
	struct Data {
		Mat mean, eigenvalues, eigenvectors;

		vector<Mat> train_projections;
		vector<vector<Mat>> test_projections;
		vector<int> labels;

		void clear()
		{
			train_projections.clear();
			test_projections.clear();
			labels.clear();
		}
	};


	Pca(int train_size, int test_size)
		: m_trainSize(train_size)
		, m_testSize(test_size)
		, m_dataSize(test_size + train_size)
		, m_components(0)
	{
		init();
	}

	void init();
	const Data *getData() { return &m_data; }
	void setComponents(int n)
	{
		m_components = n;
		extract();
	}

private:
	static const vector<string> m_faces;
	const int m_dataSize, m_testSize, m_trainSize;
	int m_components;
	vector<Mat> m_trainImages;
	vector<vector<Mat>> m_testImages;
	vector<int> m_labels;
	Data m_data;

	void extract();
	Mat convVec(const vector<Mat> &images);
	Mat convMat(const Mat &image);
};

#endif
