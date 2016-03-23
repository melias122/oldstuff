#include <iostream>
#include <set>
#include <map>
#include <list>
#include <queue>
#include <vector>
#include <algorithm>

#include "graph.h"

using namespace std;

vector<vertex*> VertexVector;	// vektor vzdialenosti

bool sortfunction (vertex* p1, vertex* p2) {
	return p1->dist< p2->dist;
}

void dijkstra(graph &gr, string start) {
	set<vertex*>::iterator it;

	// nastavime  vsetky vzdialenosti na maximalnu hodnotu - a pridame ho do mnoziny vrcholov
	for (it = (gr.allVerteces).begin(); it != (gr.allVerteces).end(); it++)
	{
		(*it)->dist = 10000;
		VertexVector.push_back(*it);
	}

	gr.string_to_Verteces[start]->dist = 0;

	while( !VertexVector.empty() ) {
		// opat utriedime podla aktualnych vzdialenosi od zaciatku
		sort (VertexVector.begin(), VertexVector.end(), sortfunction);
		// najdeme najblizsi -> nova aktualna polozka
		vertex* pActual = *VertexVector.begin();
		// a jeho vzdialenost od zaciatku je uz definitivna - vypiseme ju
		cout << pActual->name << " vzdialenost je: " << pActual->dist << endl;

		// v cykle aktualizujem vzdialenosti vsetkych jeho susedov
		set<edge*>::iterator eit;

		for ( eit=pActual->edges.begin() ; eit != pActual->edges.end(); eit++ ) {
				vertex* pNeighbor = (*eit)->getOposite(pActual);
				int dist = pActual->dist + (*eit)->dist;
				if (pNeighbor->dist > dist) {
					pNeighbor->dist = dist;
				}
		}

		// nakoniec smernik na aktualny vrchpl zo zoznamu odstranime
		VertexVector.erase(VertexVector.begin());
	}

}



int main() {

	graph siet;

	siet.addEdge("BA", "TT", 50);
	siet.addEdge("BA", "MA", 30);
	siet.addEdge("TT", "MA", 40);
	siet.addEdge("TT", "NR", 25);
	siet.addEdge("BA", "NR", 80);

	dijkstra(siet,"BA");



	return 0;
}
