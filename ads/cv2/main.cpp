#include <iostream>
#include <vector>

#include <stdlib.h>

using namespace std;

bool rozklad(vector<int>::iterator begin, vector<int>::iterator end, int c){
    if(begin == end){
        return false;
    }
    int c0 = c - (*begin);
    if(c0 > 0){
        bool ok = rozklad(begin+1, end, c0);
        if(ok){
            return true;
        }else{
            return rozklad(begin+1, end, c);
        }
    }else if(c0 == 0){
        return true;
    }
    return false;
}

int main() {

    srand(0);

    int r = 0;
    vector<int> v;

    for(int i = 0; i < 100; i++){
        r += rand() % 100;
        v.push_back(r);
    }
    cout << rozklad(v.begin(), v.end(), 10000) << endl;
    return 0;
}

