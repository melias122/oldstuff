#include <iostream>
#include <fstream>
#include <algorithm>
#include <string>
#include <vector>
#include <map>
#include <math.h>
#include <time.h>
#include <cstdlib>

using namespace std;

using Dictionary = map<string, unsigned>;
using Key = map<char, char>;

void hillClimb(string &ZT, Dictionary &dic);

void help(){
    cout << "Help" << endl
         << "./dec -f cesta_k_suboru_pre_analyzu" << endl
         << "./dec -e cesta_k_ot_vylepsena_sifra" << endl
         << "./dec -d cesta_k_zt_vylepsena_sifra" << endl;
}

void menu(){
    cout << "p - Obsah suboru" << endl
         << "q - Koniec" << endl
         << "a - Odstran vsetky znaky okrem [a-zA-Z]" << endl
         << "n - Odstran vsetky znaky okrem [0-9a-zA-Z]" << endl
         << "h - HillClimb" << endl
         << "o - Povodny text" << endl
         << "f - Frekvencna analyza" << endl
         << "2 - Bigram" << endl
         << "3 - Trigram" << endl
         << "m - Menu" << endl;
}

void delChars(string &strMod, int (*isChar)(int ch)){
    string tStr;
    for_each(begin(strMod), end(strMod), [&tStr, &isChar](int ch){
        if(isChar(ch)){
            tStr.push_back(ch);
        }
    });
    strMod.clear();
    strMod += tStr;
}

void nGram(string &strMod, int n){

    size_t size = 0;
    multimap<int, string> mfa;
    map<string,int> fa;

    for(size_t i = 0; i < strMod.size()-n+1; ++i){

        string nGram;

        for(int j=0; j < n; ++j){
            nGram += strMod[i+j];
        }

        if(fa.find(nGram) == fa.end())
            fa.insert(pair<string, int>(nGram, 1));
        else
            ++fa[nGram];
    }

    //sort
    for(auto &pr : fa){
        size += get<1>(pr);
        mfa.insert(pair<int,string>(get<1>(pr),get<0>(pr)));
    }
    long fmt=0;
    for_each(begin(mfa), end(mfa), [&size, &fmt](pair<int, string> pr){
        cout << get<1>(pr) << ": " << get<0>(pr) << "\t" << "(" << (get<0>(pr)/(double)size)*100 << ")";
        if(++fmt%3 == 0)
            cout << endl;
        else
            cout << "\t";
    });

    if(n == 1){
        cout << "Velkost abecedy: " << mfa.size() << endl;
    }
}

Key newKey(){
    Key k;
    vector<char> alpha({'a','b','c','d','e','f','g','h','i','j','k','l','m','n','o','p','q','r','s','t','u','v','w','x','y','z'});
    vector<char> alphaRand(alpha);

    random_shuffle(alphaRand.begin(), alphaRand.end(), [](int i){
        return rand()%i;
    });

    for_each(alpha.begin(), alpha.end(), [&k, &alphaRand](char ch){
        k.insert(pair<char,char>(ch, alphaRand.back()));
        alphaRand.pop_back();
    });

    return k;
}

void printKey(Key &key){
    cout << "---Key begin---" << endl;
    for(auto &k : key){
        cout << get<0>(k);
    }
    cout << endl;
    for(auto &k : key){
        cout << get<1>(k);
    }
    cout << endl << "---Key end---" << endl;
}

string encrypt(string &text, Key &key){
    string encrypted;

    for_each(begin(text), end(text), [&encrypted, &key](char ch){
        encrypted += key[ch];
    });
    return encrypted;
}

string encryptBetter(string &text, Key &key){
    string encrypted;
    vector<unsigned> count(key.size(), 0);

    char klamac = key['y'];
    key['y'] = key['i'];

    for_each(begin(text), end(text), [&encrypted, &key, &count](char ch){
        if(isalpha((ch))){
            ++count[ch - 'a'];
            encrypted += key[ch];
        }
    });

    auto max = *max_element(count.begin(), count.end());
    max %= rand()%max;
    for(unsigned i=0; i < max; ++i){
        encrypted.insert(encrypted.begin() + rand()%encrypted.size(), klamac);
    }

    key['y'] = klamac;

    return encrypted;
}

string decryptBetter(string &text, Key &key){
    string decrypted;

    for_each(begin(text), end(text), [&decrypted, &key](char ch){
        if(ch != key['y']){
            decrypted += key[ch];
        }
    });

    return decrypted;
}

void hillClimb(string &ZT, Dictionary &dic){


    auto score = [&dic](string str){
        double sc = 0.0;
        for(size_t i = 0; i < str.size()-4; ++i){
            string ngram;
            for(size_t j = i; j < i+4; ++j){
                ngram += str[j];
            }
            sc += log(dic[ngram] + 1);
        }
        return sc;
    };

    auto adjKey = [](Key &key){
        int to = rand()%5;
        for(int i=0; i < to; ++i){
            int j = rand()%26;
            int k = rand()%26;
            if(j != k)
                swap(key[j], key[k]);
        }
    };

    Key key = newKey();
    double bestScore = 0.0, bestAdj, bestOveral = 0.0;
    int bestI, bestJ, adj = 0;

    bestScore = score(ZT);
    bestOveral = bestScore-1;

    for(;;){

        bestAdj = bestScore;

        for(size_t i=0; i < key.size()-1; ++i){
            for(size_t j=i+1; j < key.size(); ++j){

                Key testKey(key);
                swap(testKey[i+'a'], testKey[j+'a']);

                double sc = score(encrypt(ZT, testKey));
                if (sc > bestAdj){
                    bestAdj = sc;
                    bestI = i;
                    bestJ = j;
                }
            }
        }

        if(bestAdj > bestScore){

            swap(key[bestI + 'a'], key[bestJ + 'a']);
            bestScore = bestAdj;

            if(bestScore > bestOveral){
                adj = 0;
                bestOveral = bestScore;
                printKey(key);
                cout << "Najlepsie score = " << bestOveral << endl;
                cout << encrypt(ZT, key) << endl;
            }
        }
        else{
            if(adj < 5){
                adjKey(key);
                ++adj;
            }
            else{
                key = newKey();
                adj = 0;
            }
            bestScore = score(encrypt(ZT, key));
        }
    }
}

void run(string &filePaht){

    ifstream in(filePaht.c_str());
    ifstream dic("slovak4.dic");

    string strOrig, strMod, c;
    Dictionary dictionary;

    if(in.is_open()){
        string s;
        while(in >> s){
            strOrig += s;
        }
        in.close();
    }
    else{
        cout << "(!!!): Nepodarilo sa otvorit subor " << filePaht << endl;
        exit(-1);
    }

    if(dic.is_open()){
        string s;
        unsigned u;
        while(dic >> s >> u){
//            transform(s.begin(), s.end(), s.begin(), ::tolower);
            dictionary.insert(pair<string,unsigned>(s, u));
        }
        dic.close();
    }
    else{
        cout << "(!!!): Nepodarilo sa otvorit subor slovak4.dic" << endl;
        exit(-1);
    }

    if(strOrig.size() < 1){
        cout << "(!!!): Prazdny subor" << endl;
        exit(-1);
    }

    strMod += strOrig;
    menu();
    for(;;){

        cin >> c;
        if(c.compare("q") == 0){
            break;
        }
        else if(c.compare("m") == 0){
            menu();
        }
        else if(c.compare("a") == 0){ // Odsrani znaky okrem [a-z]
            delChars(strMod, isalpha);
            cout << strMod;
        }
        else if(c.compare("n") == 0){ // Odsrani znaky okrem [a-z0-9]
            delChars(strMod, isalnum);
            cout << strMod;
        }
        else if(c.compare("p") == 0){
            cout << strMod;
        }
        else if(c.compare("o") == 0){
            strMod.clear();
            strMod += strOrig;
            cout << strMod;
        }
        else if(c.compare("f") == 0){       // Frekvencna analyza
            nGram(strMod, 1);
        }
        else if(c.compare("2") == 0){       // Bigram
            nGram(strMod, 2);
        }
        else if(c.compare("3") == 0){       // Trigram
            nGram(strMod, 3);
        }
        else if(c.compare("h") == 0){       // HillClimb
            hillClimb(strMod, dictionary);
        }
    }
}

int main(int argc, char *argv[]) {

    srand(unsigned(time(0)));

    if(argc != 3){
        help();
    } else if((string(argv[1]).compare("-f") == 0) && (string(argv[2]).size() > 0)) {
        string filePath = string(argv[2]);
        run(filePath);
    }

    // vylepsene sifrovanie
    else if((string(argv[1]).compare("-e") == 0) && (string(argv[2]).size() > 0)){       // encryptBetter

        ifstream in(argv[2]);
        string strOrig;

        if(in.is_open()){
            string s;
            while(in >> s){
                strOrig += s;
            }
            in.close();
        }
        else{
            cout << "(!!!): Nepodarilo sa otvorit subor " << string(argv[2]) << endl;
            exit(-1);
        }

        Key k = newKey();
        printKey(k);
        cout << encryptBetter(strOrig, k) << endl;
    }

    // vylepsene desifrovanie
    // subor musi byt v tvare
    // 1. riadok kluc(len substitucie)
    // 2. riadok text
    else if((string(argv[1]).compare("-d") == 0) && (string(argv[2]).size() > 0)){       // DecryptBetter

        ifstream in(argv[2]);
        string strOrig, key;

        if(in.is_open()){
            string s;
            in >> key;
            while(in >> s){
                strOrig += s;
            }
            in.close();
        }
        else{
            cout << "(!!!): Nepodarilo sa otvorit subor " << string(argv[2]) << endl;
            exit(-1);
        }

        Key k;
        int i=0;
        for_each(key.begin(), key.end(), [&k, &i](char ch){
            k.insert(pair<char, char>(ch, i+'a'));
            ++i;
        });

        cout << decryptBetter(strOrig, k) << endl;
    }
    return 0;
}

