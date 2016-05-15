#include <iostream>

#include "mahal.h"
#include "euclid.h"
#include "svm.h"
#include "pca.h"

constexpr int crossvalidations = 5;
constexpr int maxComponents = 50;

vector<double> mean(const vector<vector<double>> &matrix)
{
	vector<double> dst(maxComponents, 0.0);

	for (const auto &vec : matrix) {
		int i = 0;
		for (const auto &item : vec) {
			dst[i] += item;
			i++;
		}
	}

	for (auto &item : dst)
		item /= double(crossvalidations);

	return dst;
}

int main(int argc, const char *argv[])
{
	int trainSize = 10;
	int testSize = 2;

	vector<vector<double>> euclids(crossvalidations, vector<double>(maxComponents, 0.0));
	vector<vector<double>> svms(crossvalidations, vector<double>(maxComponents, 0.0));
	vector<vector<double>> mahals(crossvalidations, vector<double>(maxComponents, 0.0));

	Pca pca(trainSize, testSize);
	const Pca::Data *pca_data = pca.getData();

	Euclid euclid(pca_data);
	Svm svm(pca_data);
	Mahal mahalanobis(pca_data);

	for (int i = 0; i < crossvalidations; i++) {
		for (int j = 1; j <= maxComponents; j++) {
			pca.setComponents(j);

			euclids[i][j - 1] = euclid.classify();
			svms[i][j - 1] = svm.classify();
			mahals[i][j - 1] = mahalanobis.classify();
		}
		pca.init();
	}

	vector<double> meanEuclid = mean(euclids);
	vector<double> meanSvm = mean(svms);
	vector<double> meanMahal = mean(mahals);

//	cout << "Euclid,Svm,Mahal" << endl;
	for (int i = 0; i< maxComponents; i++) {
		cout << meanEuclid[i] << "," << meanSvm[i] << "," << meanMahal[i] << endl;
	}

	return 0;
}
