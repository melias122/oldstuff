#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <limits>
#include <algorithm>

using namespace std;

typedef vector<int> vi;
typedef vector< vi > v2i;

void
fatal(string err){
    cout << err << endl;
    exit(1);
}

void
printv2i(v2i &v){
    for(int i=0;i<v.size();i++){
        char sp = 0;
        for(int j=0;j<v[i].size();j++){
            cout << sp << v[i][j];
            sp = ' ';
        }
        cout << endl;
    }
}

void
read(v2i &v, string input) {

    ifstream in(input);
    if(!in.is_open()){
        fatal("cant open file");
    }
    int row, col;

    in >> row >> col;

    v.resize(row);
    for(int i = 0; i < v.size(); i++){
        v[i].resize(col);
        for(int j = 0; j < v[i].size(); j++){
            int k;
            in >> k;
            v[i][j] = k;
        }
    }
}

int
mean(vi &v, int a, int b) {
    int sum = 0, i = 0;
    for(; a < b; a++, i++){
        sum += v[a];
    }
    return int(sum/i);
}

int
closest(vi &v){

    int mean0 = mean(v, 0, v.size());
    int min = numeric_limits<int>::max();

    int j;

    for(int i=0; i < v.size(); i++){
        int min0 = v[i] - mean0;
        if(min0 < min) {
            min = min0;
            j = i;
        }
    }

    return j;
}

int
run(v2i &v){
    int sum = 0;
    for(int i=0; i<v.size();i++){

        int idx = closest(v[i]);

        int c = v[i][idx];

        if(sum < c) {
            sum += c;
            cout << c << endl;
        } else {
            sum -= c;
            cout << -c << endl;
        }

    }

    return sum;
}

int
closest2(vi &v, int to){

    to = abs(to);

    int min = numeric_limits<int>::max();

    int j = 0;

    for(int i=0; i < v.size(); i++) {
        int min0 = abs(v[i] - to);
        if(min0 < min) {
            min = min0;
            j = i;
        }
    }

    return j;
}

int
run2(v2i &v){
    int sum = 0;
    for(int i=0; i<v.size();i++){

        //        int idx = closest2(v[i], sum);
        cout << idx << endl;

        int k = 0;
        for(int j = 0; j < v[i].size(); j++){
            int a = abs(abs(sum) - v[i][j]);
//            int b = abs(abs(sum) - v[i][j+1]);
            if(abs(abs(sum) - v[i][j]) < min){
            }
        }

        int c = v[i][idx];

        if(abs(sum - c) < abs(sum + c)){
            sum -= c;
        } else{
            sum += c;
        }
    }

    return sum;
}

int
main() {
    string path("sum_is_zero_plusminus.txt");
    v2i v;
    read(v, path);
    int r = run2(v);
    cout << r << endl;

    return 0;
}

