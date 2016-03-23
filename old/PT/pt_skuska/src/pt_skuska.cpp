#include <iostream>
#include <fstream>
#include <map>
#include <set>
#include <vector>
#include <string>
#include <math.h>
#include <algorithm>

using namespace std;

class uzol;

class hrana {

public:

	uzol* from;			// smernik na zaciatocny vrchol
	uzol* to;			// smernik na koncovy vrchol
	double dist;		// vzdialenost

	hrana(uzol *f, uzol *t, double d){
		from = f;
		to = t;
		dist = d;
	}
};

class uzol {

public:

	string meno;
	double x;
	double y;
	vector<hrana*> hrany;

	uzol(string m, double x, double y){
		meno = m;
		this->x = x;
		this->y = y;
	}
	uzol(){}
};

bool sortfunc(hrana *a, hrana *b){
	return a->dist < b->dist;
}

// Trieda reprezentujuca graf
class Graf {
public:

	map<string, uzol*> uzly;
//	vector<hrana *> hrany;

	Graf(){}

	Graf(ifstream &f){

		string meno, s2, s3;
		double x, y;

		// pridam uzly
		while(!f.eof()){
			f >> meno >> s2 >> s3 >> x >> y;
			add_uzol(meno, x, y);
		}

		// prepocitam hrany medzi uzlami
		prep_hrany();
		usporiadaj();
	}

	// Prida  do grafu novy uzol,
	void add_uzol(string name, double x, double y){
		uzly.insert(pair<string, uzol*>(name, new uzol(name, x, y)));
	}

	void prep_hrany(){

		for(map<string, uzol*>::iterator it1 = uzly.begin(); it1 != uzly.end(); it1++){

			for(map<string, uzol*>::iterator it2 = uzly.begin(); it2 != uzly.end(); it2++){
				if(it1->second != it2->second)
					it1->second->hrany.push_back(new hrana(it1->second, it2->second, this->dist(it1->second, it2->second)));
			}
		}
	}

	void usporiadaj(){

		map<string, uzol*>::iterator it;

		for(it = uzly.begin(); it != uzly.end(); it++){
			sort(it->second->hrany.begin(), it->second->hrany.end(), sortfunc);
		}
	}

	double dist(uzol *a, uzol *b){
		return sqrt( ( pow(a->x - b->x, 2) + pow(a->y - b->y, 2) ) );
	}

	void najblizsie(int n){
		map<string, uzol*>::iterator it;

		for(it = uzly.begin(); it != uzly.end(); it++){
			cout << it->first << " ";
			for(int i = 0; i < n; i++){
				cout << it->second->hrany[i]->to->meno << " ";
			}
			cout << endl;
		}
	}

	bool suvislost(map<string, uzol*> uzly){

//		return true;
	}

	hrana *min_hrana(vector<hrana *> h){

	}

	void Kruskal(Graf &g){

		map<string, uzol*>::iterator it;
		vector<hrana*> h;

		// skopirujem si hrany
		for(it = g.uzly.begin(); it != g.uzly.end(); it++){
			for(unsigned i = 0; i < it->second->hrany.size(); i++){
				h.push_back(it->second->hrany[i]);
			}
		}
		// pridavam hrany do noveho grafu az kym nie je suvisly
		hrana *min;
		while(!suvislost(g.uzly)){
			// najdem si najmensiu hranu
			min = min_hrana(h);
			uzly.insert(pair<string, uzol*>());
		}
	}

};


int main(void) {

	ifstream stanice("suradnice.txt");

	Graf G1(stanice), G2;
	G1.najblizsie(3);
	G2.Kruskal(G1);
//	G2.dosirky("ACTON");
//	uzol* stred = G2.stred();
//	G2.dosirky(stred.name);

	return 0;
}
