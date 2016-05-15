#ifndef svm_h
#define svm_h

#include "pca.h"

class Svm
{
public:
	Svm(const Pca::Data *data)
		: data(data)
	{
	}
	double classify();

private:
	const Pca::Data *data;
};


#endif
