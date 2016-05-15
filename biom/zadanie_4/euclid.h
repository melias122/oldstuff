#ifndef euclid_h
#define euclid_h

#include "pca.h"

class Euclid
{
public:
	Euclid(const Pca::Data *data)
		: data(data)
	{
	}
	double classify();

private:
	const Pca::Data *data;

	int predict(const Mat &projection);
};

#endif
