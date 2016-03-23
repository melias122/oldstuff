#include <iostream>
#include <set>
#include <map>
#include <list>

#include "graph.h"


using namespace std;


///////////////////////////////////////////////////////////////////////////////
// implementacia triedy graph
///////////////////////////////////////////////////////////////////////////////

// Prida  do grafu novy uzol,
// Ak sa v nom uz uzol s dany menom nachadza neurobi nic.
void graph::addVertex(string name) {
	if (string_to_Verteces.find(name)==string_to_Verteces.end()) {
		string_to_Verteces[name]= new vertex(name);
		allVerteces.insert(string_to_Verteces[name]);
	}
}

// Prida do grafu novu hranu medzi uzlami
// Ak sa uzly v grafe nenachadzaju vytvori ich.
void graph::addEdge(string fromName, string toName, int dist) {
	// Ak sa uzly v grafe nenachadzaju vytvori ich.
	vertex* pFrom;
	if (string_to_Verteces.find(fromName) != string_to_Verteces.end())
	{
		pFrom = string_to_Verteces[fromName];
	}
	else {
		pFrom = string_to_Verteces[fromName] = new vertex(fromName);
	}
	allVerteces.insert(pFrom);

	vertex* pTo;
	if (string_to_Verteces.find(toName) != string_to_Verteces.end())
	{
		pTo = string_to_Verteces[toName];
	}
	else {
		pTo = string_to_Verteces[toName] = new vertex(toName);
	}
	allVerteces.insert(pTo);

	// vytvori hranu
	edge* pEdge = new edge(pFrom, pTo, dist);
	// a prida ju do mnoziny hran oboch koncovych vrcholov
	pFrom->edges.insert(pEdge);
	pTo->edges.insert(pEdge);
	// a zoznamu vsetkych hran
	allEdges.push_back(pEdge);
}




/*int main() {

	graph siet;

	siet.addEdge("BA", "TT", 50);
	siet.addEdge("BA", "MA", 30);
	siet.addEdge("TT", "MA", 40);
	siet.addEdge("TT", "NR", 25);
	siet.addEdge("TT", "NR", 25);

	map<string,vertex *>::iterator it;

	cout << "Mesta:" << endl;
	for ( it=siet.allVerteces.begin() ; it != siet.allVerteces.end(); it++ ) {
		vertex *pv = (*it).second;
		cout << pv->name << " ==> ";

		set<edge *>::iterator eit;
		for (eit = pv->edges.begin(); eit != pv->edges.end(); eit++) {
			edge *ep = *eit;
			vertex *sused = ep->getOposite(pv);
			cout << " " << sused->name << ":" << ep->dist;
		}
		cout << endl;
	}

	cout << endl;


	return 0;
}*/
