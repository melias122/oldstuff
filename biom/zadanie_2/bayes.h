#ifndef BAYES_H
#define BAYES_H

#include "qstring.h"
#include "qhash.h"
#include "qlist.h"

#include "opencv2/core.hpp"


using Vec = QList<int>;
using Vec2D = QList<Vec>;
using Data = QHash<QString, Vec2D>;

class Bayes
{
private:
	struct gaussian {
		void add(const Vec &vec);
		int size();
		double mP;
		QList<double> mMean;
		QList<double> mVariance;
	};

public:
	static cv::Ptr<Bayes> train(Data &data, int nTrain = -1);
	QString predict(const Vec &data);

private:
//	Bayes(){}
	void add(const QString &k, const gaussian &g);

private:
	QHash<QString, gaussian> klassifier;
};

#endif // BAYES_H
