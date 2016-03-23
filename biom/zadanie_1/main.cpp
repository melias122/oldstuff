#include "opencv2/core.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"

#include <iostream>
#include <fstream>
#include <sstream>

#include "qt5/QtCore/qdir.h"

using namespace cv;
using namespace std;

void
read(Mat &train, Mat &test) {

    QDir dir(QString("db/"));
    QStringList files = dir.entryList({"*.png"}, QDir::Files);

    if(files.empty()){
        cout << "empty db" << endl;
        exit(1);
    }

    // 15 subjektov
    for(int i = 0; i < 15; i++) {
        vector<Mat> v;
        v.reserve(11);
        // 11 tvari
        for(int j = 0; j < 11; j++){
            int k = (i*11) + j;
            Mat img = imread(QString("db/" + files[k]).toStdString(), 0);
            img = img.reshape(1, 1);

            Mat m;
            img.convertTo(m, CV_32F);
            v.push_back(m);
        }
        random_shuffle(v.begin(), v.end());

        while(v.size() > 0){
            Mat m = v.back();
            if(v.size() <= 3) {
                test.push_back(m);
            }else{
                train.push_back(m);
            }
            v.pop_back();
        }
    }
}

void
FMR_FNMR(PCA &pca, Mat &train, Mat &test, double step = 0.05, double distance = 44000.0){

    struct stat {
        double total{0}, match{0};
        double res() { return match/total; }
    };

    Mat w, r;

    cout << "Threshold;" << "FMR;" << "FNMR;" << "ROC TP;" << "ROC FP"  << endl;

    for(double t = 0.0; t <= 1.000001; t += step) {
        double min = distance * t;

        stat fmr, fnmr;

        for(int i = 0; i < 15; i++){
            for(int j = 0; j < 3; j++){

                Mat face = test.row((i*3) + j);

                pca.project(face, w);
                pca.backProject(w, r);

                for(int k = 0; k < 15; k++) {
                    if(k == i){
                        // FNMR
                        for(int l = 0; l < 8; l++) {
                            double d = norm(r, train.row((k*8) + l));
                            if(d > min){
                                fnmr.match++;
                            }
//                            else {
                                // ROC TP
//                            }
                            fnmr.total++;
                        }

                    } else {
                        // FMR
                        for(int l = 0; l < 8; l++){
                            double d = norm(r, train.row((k*8) + l));
                            if(d < min){
                                fmr.match++;

                                // ROC FP
                            }
                            fmr.total++;
                        }
                    }
                }
            }
        }

        cout << 1-t << ";"
             << fmr.res() << ";"
             << fnmr.res() << ";"
             << ((fnmr.total-fnmr.match)/fnmr.total) << ";"
             << fmr.res() << endl;
    }
}

int
predict(PCA &pca, Mat &train, Mat face, double threshold = 0.3, double distance = 44000){
    Mat w = pca.project(face);
    Mat predicted = pca.backProject(w);

    int label = -1;
    double min = distance * threshold;

    for(int i = 0; i < 15; i++){
        for(int j = 0; j < 8; j++){
            double d = norm(predicted, train.row((i*8) + j));
            if(d < min){
                min = d;
                label = i;
            }
        }
    }

    return label;
}

int
main(void) {

    int maxComponents = 85;

    Mat train, test;
    read(train, test);

    PCA pca(train, Mat(), PCA::DATA_AS_ROW, maxComponents);

    //    cout << "Predicting subject " << predict(pca, train, test.row((3*0)+0)) + 1 << endl;

    FMR_FNMR(pca, train, test);

    return 0;
}
