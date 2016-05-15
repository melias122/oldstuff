#include <iostream>
#include <string>

#include "time.h"

#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>

#include "pca.h"
#include "euclid.h"
#include "hamming.h"
#include "hybrid.h"

using std::vector;
using cv::Mat;
using std::string;

vector<Iris> iris_train;
vector<vector<Iris>> iris_test;

vector<Mat> faces_train;
vector<vector<Mat>> faces_test;
vector<int> faces_labels;

#define TrainSizeEachPerson (4)
#define TestSizeEachPerson (4)

void fatal(string msg)
{
	std::cout << msg << std::endl;
	exit(1);
}

void readFaces(vector<Mat> &train, vector<vector<Mat>> &test, vector<int> &labels)
{
	for (int person = 1; person <= 10; person++) {
		vector<Mat> images;
		for (int foto = 1; foto <= 7; foto++) {
			string face("../data/" + std::to_string(person) + "/face/" + std::to_string(foto) + ".bmp");

			Mat image = cv::imread(face, cv::IMREAD_GRAYSCALE);
			if (image.data == NULL) {
				fatal("cant read " + face);
			}
			images.push_back(image);
		}

		test.push_back({});
		random_shuffle(begin(images), end(images));
		for (int i = 0; i < images.size(); i++) {
			if (i < TestSizeEachPerson) {
				test.back().push_back(images[i]);
			} else {
				train.push_back(images[i]);
				labels.push_back(person);
			}
		}
	}
}

void readIris(vector<Iris> &train, vector<vector<Iris>> &test)
{
	for (int person = 1; person <= 10; person++) {
		vector<Iris> images;
		for (int foto = 1; foto <= 7; foto++) {
			string iris_mask("../data/" + std::to_string(person) + "/iris/" + std::to_string(foto) +
					 "_mask.bmp");
			string iris_temp("../data/" + std::to_string(person) + "/iris/" + std::to_string(foto) +
					 "_template.bmp");

			Mat mask = cv::imread(iris_mask, cv::IMREAD_GRAYSCALE);
			Mat temp = cv::imread(iris_temp, cv::IMREAD_GRAYSCALE);

			if (mask.data == NULL || temp.data == NULL) {
				fatal("cant read iris " + std::to_string(person) + "_" + std::to_string(foto));
			}

			images.push_back(Iris(temp, mask));
			images.back().label = person;
		}

		test.push_back({});
		random_shuffle(begin(images), end(images));
		for (int i = 0; i < images.size(); i++) {
			if (i < TestSizeEachPerson) {
				test.back().push_back(images[i]);
			} else {
				train.push_back(images[i]);
			}
		}
	}
}

void iris(int in)
{
	Hamming hamming(iris_train);

	Iris iris = iris_test[in - 1][rand() % iris_test[in - 1].size()];

	std::cout << "predicted: " << hamming.predict(iris).first << "\n";
}

void face(int in, int dim)
{
	Pca pca(faces_train, faces_test, faces_labels, dim);
	Euclid euclid(pca.data());

	Mat face = pca.data()->test[in - 1][rand() % pca.data()->test[in - 1].size()];

	std::cout << "predicted: " << euclid.predict(face).first << "\n";
}

void hybrid(int in, int dim)
{
	Hamming hamming(iris_train);
	Pca pca(faces_train, faces_test, faces_labels, dim);
	Euclid euclid(pca.data());
	Hybrid hybrid(&euclid, &hamming);

	Iris iris = iris_test[in - 1][rand() % iris_test[in - 1].size()];
	Mat face = pca.data()->test[in - 1][rand() % pca.data()->test[in - 1].size()];

	std::cout << "predicted: " << hybrid.predict(face, iris).first << "\n";
}

void graphs()
{
	struct stat {
		double total{0}, match{0};
		double fmr() { return match / total; }
		double rocFP() { return fmr(); }
		double fnmr() { return fmr(); }
		double rocTP() { return ((total - match) / total); }
	};

	Hamming hamming(iris_train);
	Pca pca(faces_train, faces_test, faces_labels);
	Euclid euclid(pca.data());
	Hybrid hybrid(&euclid, &hamming);

	//	double TH = 950; // hamming
	double TH = 0.01;  // euclid
	for (int t = 0; t < 100; t++) {

		stat fmr, fnmr;

		for (int i = 0; i < iris_test.size(); i++) {

			// concrete person
			//		int faceLabel = faces_labels[i];
			for (int j = 0; j < iris_test[i].size(); j++) {

				int label = iris_test[i][j].label;
				Iris iris = iris_test[i][j];
				Mat face = pca.data()->test[i][j];

				//				{  // hamming test
				//					int label = iris_test[i][j].label;

				//					auto hp = hamming.predict(iris_test[i][j]);

				//					if (hp.second > TH && hp.first == label) {
				//						// FNMR
				//						fnmr.match++;
				//					}
				//					if (hp.second < TH && hp.first != label) {
				//						// FMR
				//						fmr.match++;
				//					}
				//					fnmr.total++;
				//					fmr.total++;
				//				}

				//				{  // euclid test
				//					int label = pca.data()->labels[i];

				//					auto hp =
				// euclid.predict(pca.data()->test[i][j]);

				//					if (hp.second > TH) {
				//						if (hp.first == label) {
				//							// FNMR
				//							fnmr.match++;
				//						}
				//						fnmr.total++;
				//					} else {
				//						if (hp.first != label) {
				//							// FMR
				//							fmr.match++;
				//						}
				//						fmr.total++;
				//					}
				//				}

				{  // hybrid test
					auto hp = hybrid.predict(face, iris);

					if (hp.second > TH) {
						if (hp.first == label) {
							// FNMR
							fnmr.match++;
						}
						fnmr.total++;
					} else {
						if (hp.first != label) {
							// FMR
							fmr.match++;
						}
						fmr.total++;
					}
				}
			}
		}

		std::cout << TH << " " << fmr.fmr() << " " << fnmr.fnmr() << " " << fnmr.rocTP() << " " << fmr.rocFP()
			  << "\n";

		TH += 0.0005;
	}
	std::cout.flush();
}

int main(int argc, char **argv)
{
	srand(time(0));
	if (argc < 3) {
		printf("Usage: %s <mode> <predict_person_id> <optional_components>\nmode:I (Iris), F (Face), H (I+F)\n", argv[0]);
		return 1;
	}

	readIris(iris_train, iris_test);
	readFaces(faces_train, faces_test, faces_labels);

	//	graphs();

	int id = std::stoi(string(argv[2]));

	int dim = 50;
	if (argc > 4) {
		dim = std::stoi(string(argv[2]));
	}

	switch (argv[1][0]) {
	case 'I':
		iris(id);
		break;

	case 'F':
		face(id, dim);
		break;

	case 'H':
		hybrid(id, dim);
		break;

	case 'G':
		graphs();

	default:
		printf("Usage: %s <mode> <predict_person_id> <optional_components>\nmode:I (Iris), F (Face), H (I+F)\n", argv[0]);
		return 1;
	}

	return 0;
}
