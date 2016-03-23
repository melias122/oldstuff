#include "qstring.h"
#include "qfile.h"
#include "qtextstream.h"
#include "qdebug.h"
#include "qstringlist.h"

#include "bayes.h"

#include "opencv2/core.hpp"
#include "opencv2/ml.hpp"

void load(QString filename, Data *train, Data *test, int testSamplesEach = 50)
{
	QFile f(filename);
	if(!f.open(QFile::ReadOnly)){
		qWarning() << "can't open " << filename;
		return;
	}
	QTextStream s(&f);

	Data raw;
	while(!s.atEnd()){

		auto line = s.readLine().split(" ");

		QString key;
		Vec vec;

		for(int i = 0; i < line.size(); i++){
			if(i == 0){
				key = line[i];
				continue;
			}
			vec.append(line[i].toInt());
		}

		raw[key].append(vec);
	}

	for(auto &key : raw.keys()){

		Vec2D te;
		for(int i = 0; i < testSamplesEach && !raw[key].empty(); i++){
			Vec v = raw[key].takeFirst();
			te.append(v);
		}
		Q_ASSERT(te.size() == testSamplesEach);

		test->insert(key, te);

		Vec2D tr;
		while(!raw[key].empty()){
			Vec v = raw[key].takeFirst();
			tr.append(v);
		}
		train->insert(key, tr);
	}
}

void dataToMat(Data &data, cv::Mat &samples, cv::Mat &labels, int nSamples = -1)
{
	int i = 0;
	for(auto k : data.keys()) {
		if(i == nSamples){
			i = 0;
			continue;
		}
		i++;

		cv::Mat label(1, labels.cols, labels.type());
		label.at<int>(0, 0) = int(*k.toStdString().c_str());

		for(auto &s: data[k]) {
			cv::Mat sample(1, samples.cols, samples.type());
			for(int i = 0; i < s.size(); i++){
				sample.at<float>(0, i) = float(s[i]);
			}
			samples.push_back(sample);
			labels.push_back(label);
		}
	}
}

struct stat {
	stat(int nTrainSamples)
		: nTrainSamples(nTrainSamples)
	{}

	double rate() {
		double r = double(nClassified)/double(nTestSamples);
		r *= 100;
		return r;
	}

	int nTrainSamples{0};
	int nTestSamples{0};
	int nClassified{0};
};

void graph()
{
	Data bayesTrainData, bayesTestData;
	load("pismena.txt", &bayesTrainData, &bayesTestData, 500);

	cv::Mat svmTestSamples(0, 16, CV_32FC1), svmTestLabels(0, 1, CV_32SC1);
	dataToMat(bayesTestData, svmTestSamples, svmTestLabels);

	int ntk = bayesTrainData.keys().size();

	for(int i = 1; i <= 15; i++){

		int nTrainSamplesEach = i;
		stat statBayes(nTrainSamplesEach * ntk);
		stat statSvm(nTrainSamplesEach * ntk);

		// Bayes
		auto bayes = Bayes::train(bayesTrainData, nTrainSamplesEach);

		for(auto &w : bayesTestData.keys()){
			for(auto &sample : bayesTestData[w]){
				auto predicted = bayes->predict(sample);
				if(predicted == w){
					statBayes.nClassified++;
				}
				statBayes.nTestSamples++;
			}
		}

		// Svm
		cv::Mat svmTrainSamples(0, 16, CV_32FC1), svmTrainLabels(0, 1, CV_32SC1);
		dataToMat(bayesTrainData, svmTrainSamples, svmTrainLabels, nTrainSamplesEach);
		auto svmTrainData = cv::ml::TrainData::create(svmTrainSamples, cv::ml::ROW_SAMPLE, svmTrainLabels);
		auto svm = cv::ml::SVM::create();

		svm->setType(cv::ml::SVM::C_SVC);
		svm->setC(0.3);
//		svm->setKernel(cv::ml::SVM::CHI2);
		svm->setKernel(cv::ml::SVM::INTER);
		svm->setTermCriteria(cv::TermCriteria(cv::TermCriteria::MAX_ITER, (int)1e3, 1e-6));
		svm->train(svmTrainData);

		for(int i = 0; i < svmTestSamples.rows; i++){
			auto predicted = svm->predict(svmTestSamples.row(i));
			auto w = svmTestLabels.at<int>(i, 0);

			if(predicted == w){
				statSvm.nClassified++;
			}
			statSvm.nTestSamples++;
		}

		// Stats
		std::cout << statBayes.nTrainSamples << " "
				  << statBayes.rate() << " "
				  << statSvm.nTrainSamples << " "
				  << statSvm.rate()
				  << std::endl;
	}
}

int main(void)
{
	graph();
	return 0;
}

