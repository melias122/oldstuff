#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

typedef struct _node{
	unsigned int idStret;
	unsigned int casOD;
	unsigned int casDO;
	unsigned long priorita; // Priorita uzlu
	struct _node * parent;
	struct _node * left;
	struct _node * right;
}treap;

// Prototypy
int getPriority(void);
void insert_treap(unsigned int casOD, unsigned int casDO, unsigned int idStret);
void buble_up(treap * node);
void insert_id(treap * node, unsigned int id);
void ftreeinterval(unsigned int casOD, unsigned int casDO, treap * node);
void findinterval(unsigned int casOD, unsigned int casDO);
void ftreepoint(unsigned int cas, treap * node);
void quit(void);
void findpoint(unsigned int cas);
int comparator(const void * a, const void * b);
void lrotate(treap * node);
void rrotate(treap * n);
void buble_down(treap * node);
void delete_by_id(unsigned short cislo_schodzky);

// Globalne premmene
treap           * root      = NULL;
unsigned long   * pole_id   = NULL;
unsigned int    * id        = NULL;
unsigned int      max       = 0;
unsigned int      pocet     = 0;


// Funkcia vrati nahodne cislo
// v intervale (0,2^31 - 1)
int getPriority(){
	return rand()%2147483643 - 1;
}

// Pomocna funkcia pre quicksort
// Zdroj : http://www.cplusplus.com/reference/clibrary/cstdlib/qsort/
int comparator(const void * a, const void * b) {
    return ( *(int*)a - *(int*)b );
}

// Id schozok si vkladam do pola
void insert_id(treap * node, unsigned int id) {
    
    unsigned long   * pole_id_realloc   = NULL;
    
    if(id > max){
        
        max = id;
        
        if (pole_id == NULL) {
            pole_id = (unsigned long *) malloc(id * sizeof(unsigned long));
        }
        else {
            pole_id_realloc = (unsigned long *) realloc(pole_id, id * sizeof(unsigned long));
            pole_id = pole_id_realloc;
        }
    }
    pole_id[id - 1] = (unsigned long)node;
}

// Vkladanie do stromu
void insert_treap(unsigned int casOD, unsigned int casDO, unsigned int idStret){
    
	treap * novy 		= NULL;
	treap * pom 		= root;
    
	novy = (treap *)malloc(sizeof(treap));
    
	novy->casOD 	= casOD;
	novy->casDO 	= casDO;
	novy->idStret	= idStret;
	novy->priorita 	= getPriority();
	novy->parent 	= NULL;
	novy->right 	= NULL;
	novy->left 	= NULL;
    
	if(root == NULL){
		root 			= novy;
        insert_id(novy, idStret);
        
	}
	else {
		// Zaradenie prvku do stromu
		// zaciname od korena
		while(pom != NULL){
            
			novy->parent = pom;
            
			// Ideme vpravo
			if(casOD > pom->casOD) {
				if(pom->right ){
					pom = pom->right;
				}
				else {
					pom->right = novy;
					break;
				}
			}
            
			// Ideme vlavo
			else {
				if(pom->left ) {
					pom = pom->left;
				}
				else {
					pom->left = novy;
					break;
				}
			}
		}		
        insert_id(novy, idStret);
		buble_up(novy);
	}
}

// Lava rotacia
void lrotate(treap * node) {
    
	treap * parent  = node->parent;
	treap * child   = node->right;
	treap * two     = child->left;
    
	if(parent) {
		if(node == parent->left) {
			parent->left = child;
        }
		else {
			parent->right = child;
        	}
	} 
    	else {
        	root = child;
	}
	child->left = node;
	child->parent = parent;
    
	node->parent = child;
	node->right = two;
	if(two){
		two->parent = node;
    	}
}

// Prava rotacia
void rrotate(treap * n) {
    
	treap * parent  = n->parent;
	treap * child   = n->left;
	treap * two     = child->right;
    
	if(parent) {
		if(n == parent->left) {
			parent->left = child;
        }
		else {
			parent->right = child;
        }
    }
    else {
		root = child;
	}
	child->right = n;
	child->parent = parent;
    
	n->parent = child;
	n->left = two;
	if(two){
		two->parent = n;
    }
}

void buble_up(treap * node) {
    
	while(node->priorita >= node->parent->priorita) {
        
		if(node->priorita == node->parent->priorita) {
            
		}
        
		if(node->parent->left == node){
			rrotate(node->parent);
        }                                    
		else {
			lrotate(node->parent);
        }
		if(node->parent == NULL) {
			break;
        }
	}
}

// aka destroy
void buble_down(treap * node){
    
    if (node) {
        while(node->left || node->right) {
            
            if(node->left && node->right) {
                
                if(node->left->priorita > node->right->priorita) {
                    rrotate(node);
                }
                else {
                    lrotate(node);
                }
            }
            else if(node->left && !node->right) {
                rrotate(node);
            }
            else {
                lrotate(node);
            }
        }
        
        if(node->parent->left == node) {
            node->parent->left = NULL;
            free(node);
        } else {
            node->parent->right = NULL;
            free(node);
        }
    }
}

// Pomocna funkcia pre vyhladanie intervalu
void ftreeinterval(unsigned int casOD, unsigned int casDO, treap * node){
    
    treap * pom = node;
    
    if (pom) {
        
        // Mozu nastat 4 pripady kedy sa intervaly prekryvaju
        if (
		(casOD >= pom->casOD && casOD <= pom->casDO) ||
            	(casDO >= pom->casOD && casDO <= pom->casDO) ||
            	(pom->casOD > casOD && pom->casDO < casDO)   ||
            	(pom->casOD < casOD && pom->casDO > casDO)
	) {
            id[pocet] = pom->idStret;
            pocet++;
        }
    }
    if (pom->right) {
        ftreeinterval(casOD, casDO, pom->right);
    }
    if (pom->left) {
        ftreeinterval(casOD, casDO, pom->left);
    }
}

// Pomocna funkcia pre vyhladanie bodu
void ftreepoint(unsigned int cas, treap * node){
    
    treap * pom = node;
    
    if (pom) {
        if (cas >= pom->casOD && cas <= pom->casDO) {
            
			id[pocet] = pom->idStret;
            pocet++;
        }
        if (pom->right) {
            ftreepoint(cas, pom->right);
        }
        if (pom->left) {
            ftreepoint(cas, pom->left);
        }   
    }
}

// Vypíše vzostupne usporiadané čísla schôdok,
// ktoré prekrývajú interval OD až DO.
// Vypíše - (pomlčku), ak nie sú žiadne.
void findinterval(unsigned int casOD, unsigned int casDO){
    
    int i;
    if(max > 0)	{
        id = (unsigned int *) malloc(max * sizeof(int));
        
        // Hladanie
        ftreeinterval(casOD, casDO, root);
        
        // Usporiadanie
        qsort(id, pocet, sizeof(int), comparator);
        
        // Vypis
    }
    if (pocet == 0) {
        printf("-");
    }
    else {
        for (i = 0; i < pocet; i++) {
            if (i == 0)
                printf("%d",id[i]);
            else
                printf(" %d",id[i]);
        }
    }
    printf("\n");
    
    if(pocet > 0){
    	free(id);
    }
    pocet = 0;
}

// Vypíše vzostupne usporiadané čísla schôdok,
// ktoré zasahujú do časového okamihu CAS.
// Vypíše - (pomlčku), ak nie sú žiadne.
void findpoint(unsigned int cas){
    
    int i;
    if(max > 0){
    	id = (unsigned int *) malloc(max * sizeof(unsigned int));
        
		// Hladanie
		ftreepoint(cas, root);
        
		// Usporiadanie
		qsort(id, pocet, sizeof(int), comparator);
    }
    // Vypis
    if (pocet == 0) {
        printf("-");
    }
    else {
        for (i = 0; i < pocet; i++) {
            if (i == 0)
                printf("%d",id[i]);
            else
                printf(" %d",id[i]);        
        }
    }
    printf("\n");
    
    if(pocet > 0){
    	free(id);
    }
    pocet = 0;
}

// Zmaž z denníčka schôdzku s číslom I.
void delete_by_id(unsigned short cislo_schodzky){
    
	treap * pom = (treap *)pole_id[cislo_schodzky];
	buble_down(pom);
}

int main (int argc, const char * argv[]) {
    
    char str[15], buffer[100];
    int c_od = 0, c_do = 0, cislo = 0;
    
    while (fgets(buffer, 100, stdin)) {
        
        // Insert
        if (buffer[0] == 'I') {
            sscanf(buffer, "%s %d %d %d", str, &c_od, &c_do, &cislo);
            insert_treap(c_od, c_do, cislo);
        }
        else if (buffer[0] == 'F') {
            
            // Findpoint
            if (buffer[4] == 'P') {
                sscanf(buffer, "%s %d", str, &cislo);
                findpoint(cislo);
            }
            // Findinterval
            if (buffer[4] == 'I') {
                sscanf(buffer, "%s %d %d", str, &c_od, &c_do);
                findinterval(c_od, c_do);
            }
        }
        // Delete
        else if(buffer[0] == 'D') {
            sscanf(buffer, "%s %d", str, &cislo);
            delete_by_id(cislo);
        }
        // Quit
        else if (buffer[0] == 'Q'){
            sscanf(buffer, "%s", str);
            free(root);
            free(pole_id);
            break;
        }
    }
    return 0;
}
