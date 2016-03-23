#include "bayes.h"

#include "math.h"

static Vec2D transpose(Vec2D &data);
static double mean(const Vec &v);
static double square(double x);
static double variance(const Vec &v);
static double P(const QString &s, const Data &d);
static double G(double mean, double variance, double v);

Vec2D transpose(Vec2D &data)
{
	int size = data[0].size();

	Vec2D vec;
	vec.reserve(size);

	while(vec.size() != size){
		vec.append(Vec());
	}

	for(int i = 0; i < data.size(); i++){
		for(int j = 0; j < size; j++){
			vec[j].append(data[i][j]);
		}
	}
	return vec;
}

static double mean(const Vec &v)
{
	Q_ASSERT(v.size() != 0);

	double m = .0;
	for(auto &i : v){
		m += i;
	}
	double size = static_cast<double>(v.size());
	return m/size;
}

static double square(double x)
{
	return x * x;
}

static double variance(const Vec &v)
{
	double m = mean(v);
	double size = static_cast<double>(v.size());
	double sum = .0;
	for(auto &i : v){
		sum += square(i-m);
	}
	return sum/size;
}

static double P(const QString &s, const Data &d){
	double f = double(d[s].size());
	return f/double(d.size());
}

static double G(double mean, double variance, double v){
	double result = -1;
	result *= square(v - mean);
	result /= ( 2 * variance);
	return (1 / (sqrt(2 * M_PI * variance))) * exp(result);
}

void Bayes::gaussian::add(const Vec &vec){
	mMean.append(mean(vec));
	mVariance.append(variance(vec));
}
int Bayes::gaussian::size(){
	return mMean.size();
}

cv::Ptr<Bayes> Bayes::train(Data &data, int nTrain){
	Q_ASSERT(data.size() != 0);

	Bayes *bayes = new Bayes;

	int i = 0;
	for(auto &k : data.keys()){
		Q_ASSERT(data[k].size() != 0);

		if(i == nTrain){
			i = 0;
			continue;
		}
		i++;

		Vec2D vec = transpose(data[k]);

		gaussian dist;
		// A
		dist.mP = P(k, data);

		// B
		for(auto &v : vec){
			dist.add(v);
		}
		bayes->add(k, dist);
	}

	return bayes;
}

QString Bayes::predict(const Vec &data){
	QString prediction;
	double max = 0;
	for(auto &k : klassifier.keys()){
		auto g = klassifier[k];

		auto posterior = g.mP;
		for(int i = 0; i < g.size(); i++){
			posterior *= G(g.mMean[i], g.mVariance[i], data[i]);
		}
		if(posterior > max) {
			max = posterior;
			prediction = k;
		}
	}
	return prediction;
}

void Bayes::add(const QString &k, const gaussian &g){
	klassifier[k] = g;
}
