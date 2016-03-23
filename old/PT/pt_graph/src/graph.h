#ifndef GRAPH_H_
#define GRAPH_H_

#include <iostream>
#include <set>
#include <map>
#include <list>
#include <string>

class vertex;

// Trieda reprezentujuca vrchol grafu
class edge {
public:
	vertex* from;		// smernik na zaciatocny vrchol
	vertex* to;			// smernik na koncovy vrchol
	int		dist;		// vzdialenost


	// ctor
	edge(vertex *f, vertex *t, int d) : from(f), to(t), dist(d) {};

	// Ak *p je vrchol hrany, vrati smernik na protilahly vrchol.
	vertex* getOposite(vertex *p) {
		if (from==p) return to;
		if (to==p) return from;
		return 0;
	}
};

// Trieda reprezentujuca hranu grafu
class vertex {
public:
	std::set<edge*> edges;	// mnozina smernikov na hrany zacinajuce/konciace vo vrchole
	std::string name;		// meno vrchola
	int dist;

	// ctor
	vertex(std::string n) : name(n),dist(10000){};
};


// Trieda reprezentujuca graf
class graph {
public:
	std::map<std::string,vertex *> string_to_Verteces;
	std::set<vertex*> allVerteces;
	std::list<edge *> allEdges;

	// Prida  do grafu novy uzol,
	// Ak sa v nom uz uzol s dany menom nenachadza neurobi nic.
	void addVertex(std::string name);

	// Prida do grafu novu hranu medzi uzlami
	// Ak sa uzly v grafe nenachadzaju vytvori ich.
	void addEdge(std::string fromName, std::string toName, int dist);


};


#endif /* GRAPH_H_ */
