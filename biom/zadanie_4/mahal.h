#ifndef mahal_h
#define mahal_h

#include "pca.h"

class Mahal
{
public:
	Mahal(const Pca::Data *data)
		: data(data)
	{
	}
	double classify();

private:
	const Pca::Data *data;
	vector<Mat> m_classProjections;
	vector<Mat> m_meanProjections;

	void makeProjections();
	int predict(const Mat &projection);
	double calculate(const Mat &projection, int class_id);
};


#endif
