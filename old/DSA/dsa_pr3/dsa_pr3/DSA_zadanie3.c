//  Created by Martin Elias on 12/7/11.

#include <stdio.h>
#include <stdlib.h>

// Struct

typedef struct _META {
    unsigned short jeVolny;     // priznak
    unsigned int velkost;       // velkost bloku
    struct _META * dalsi;       // pointer na dalsi volny
} META;

// Macra
#define NEKONECNO 123456789
#define FALSE 0
#define TRUE 1


// Prototypy
void spracuj(int obchod, int s);
void dijkstra(int s, int size);
void x_swap(int *i, int *j);
void backtrack_paths(int *obchody, int zac, int koniec);
void memory_free(void *ptr);
void *memory_alloc(unsigned int size);
void memory_init(void *ptr, unsigned int size);
void collect(void);

// Globalne premmenne
int * obchody;
int ** mat;
int d2[12][12];
int * d;
META * zvp;

// Dlzka cesty je zatial neznama
int cesta = NEKONECNO;

int main () {
    
    int n;          // veslkost matice
    int m;          // pocet ciest medzi krizovatkami
    int x;          // cesta z x
    int y;          // do y
    int dlzka = 0;      // s dlzkou
    int s;          // pocet obchodov
    int i, j;
    
    void * mem = malloc(100000 * 1000 * sizeof(int));
	memory_init(mem,10000000);

	FILE * subor = fopen("/Users/melias122/dsa/test1.in","r");

    
    // Nacitanie zaciatocnych informacii
    fscanf(subor,"%d %d", &n, &m);
    
    int * matt[n + 1];
    int * dd = (int *) memory_alloc((n + 1) * sizeof(int));
    int * obhc = (int *) memory_alloc(13 * sizeof(int));
    
    // inicializacia prvkov
    for (i = 0; i < n; i++) {
        matt[i] = (int *) memory_alloc((n + 1) * sizeof(int));
    }
    mat = matt;
    d = dd;
    obchody = obhc;
    
    // Naplnenie prvkov matice nulami
    for (i = 0; i < n; i++) {
        for (j = 0; j < n; j++) {
            mat[i][j] = 0;
        }
    }
    
    
    // Naplenie matice
    for (i = 0; i < m; i++) {
        fscanf(subor,"%d%d%d", &x, &y, &dlzka);
        // Ak uz nieco mame na mieste x, y a dlzka cesty je vacsia
        // tak takuto cestu ignorujeme
        if (mat[x][y] != 0 && dlzka > mat[x][y]){
            continue;
        }
        else if(mat[y][x] != 0 && dlzka > mat[y][x]){
            continue;
        }
        else {
            mat[x][y] = dlzka;
            mat[y][x] = dlzka;
        }
    }
    
    // Nacitanie krizovatiek(obchodov)
    // ktore chceme navstivit
    fscanf(subor,"%d", &s);
    for (i = 1; i < s+1; i++) {
        fscanf(subor,"%d", &j);
        obchody[i] = j;
    }
    obchody[0] = 0;     // Zaciatok cesty
    obchody[s+1] = 0;   // Koniec nasej cesty, nakoniec musime ist domov
    
    // Dijkstrov algoritmus na riadky s obchodami
    // ktore si kopirujeme do novej matice najblizsich ciest
    for (i = 0; i <= s; i++) {
        dijkstra(obchody[i], n);
        spracuj(obchody[i], s);
    }
    
// Len pre testovanie
// Dijkstrova matica najblissich ciest
//    
//    for (i = 0; i < n; i++) {         
//        for (j = 0; j < n; j++) {
//            printf("%d ",mat[i][j]);
//        }
//        printf("\n");
//    }
    for (i = 0; i <= s; i++) {
        for (j = 0; j <= s; j++) {
            printf("%d ",d2[i][j]);
        }
        printf("\n");
    }

    for(i = 0; i <= s; i++){
    	obchody[i] = i;
    }

    // Backraking vsetkych ciest
    backtrack_paths(obchody, 1, s);
    
    // A nasa cesta
    printf("%d\n",cesta);
    
    
    free(mem);
    exit(0);
}

// Pomocna funkcia na vytvorenie matice susednosti
// obchodov
int pocet = 0;
void spracuj(int obchod, int s) {

	int i;
	for (i = 0; i <= s; i++) {
		d2[pocet][i] = d[obchody[i]];
	}
    // za kazdym spustenim to bude dalsi riadok
	pocet++;
}




// Funkcia prehlava vsetky volne bloky a spaja ich
void collect(void){
    
    META * hlavicka = zvp;
    META * pom = hlavicka->dalsi;
    META * pred = hlavicka;
    
    // Prehladavame volne bloky a spajame ich
    while (pom->dalsi != NULL) {
        
        pred = pom;
        pom = pom->dalsi;
        
        // Ak pred + meta + velkost + 1 je dalsi
        // tak ich spojime
        if ((((void *)pred) +sizeof(META) + pred->velkost + 1) == (pom)) {
            pred->velkost += sizeof(META) + pom->velkost;
            pred->dalsi = pom->dalsi;
            pom = hlavicka->dalsi; // reset a ideme od znova
        }
    }
}

void *memory_alloc(unsigned int size) {
    
    META * hlavicka     = zvp;
    META * pom          = hlavicka->dalsi;  // Najblizssi volny
    META * pred         = hlavicka;
    META * blok_new     = NULL;
    int isCollected     = 0;                // Priznak ci sme uz spustili collector
    
    // Nastavenie minimalnej velkosti
    // musi byt aspon sizeof(META) - sizeof(unsigned short)
    // aby sme pri free mohli vlozit META spat
    if (size < sizeof(META) - sizeof(unsigned short)){
        size = sizeof(META) - sizeof(unsigned short);
    }
    
    // Prehladavame ZVP
    while (pom != NULL) {
        
        if( (pom->jeVolny == TRUE) && (pom->velkost >= size)){
            break;
        }
        // Ak prejdem celu pamat a nenasiel som dostatocne velke miesto
        // velka fragmentacia spustame collector a hladame od znova
        else if (pom->dalsi == NULL && isCollected == 0) {
            collect();
            isCollected = 1;
            pom = hlavicka;
        }
        else if (pom->dalsi == NULL && isCollected == 1) {
            return  NULL;
        }
        pred    = pom;
        pom     = pom->dalsi;
    }
    
    if(pom == NULL){
        return NULL;
    }
    // Ak pom->velkost je vacsia ako (sizeof(META) + sizeof(unsigned short))
    // vytvorime nove metadata
    if(pom->velkost > (sizeof(META) + sizeof(unsigned short))) {
        
        blok_new = ((void *)pom) + (sizeof(unsigned short) + size);
        blok_new->jeVolny   = TRUE;
        blok_new->velkost   = pom->velkost - size - sizeof(unsigned short);
        
        pred->dalsi = blok_new;
        blok_new->dalsi = pom->dalsi;
    }
    // Inak aby sme neprisli neake bajty
    // size bude velkost meta
    else {
        size = pom->velkost;
        pred->dalsi = pom->dalsi;
    }
    
    // Nastavenie velkosti bloku
    *((unsigned short *)pom)    = size;
    
    // Vraciame pointer
    return ((void *)pom + sizeof(unsigned short));
}

void memory_free(void *ptr) {
    
    ptr      		= (ptr - sizeof(unsigned short));   // Musime spravit posun spat o velkost meta
    META * blok     = (META *) ptr;
    META * hlavicka = zvp;
    META * pom		= hlavicka->dalsi;
    META * pred		= hlavicka;
    
    // Predpokladame ze pointer je spravny
    // a nie je NULL
    if(ptr != NULL){
        
        while (pom != NULL) {
            
            // Prehladavame volnu pamat a hladame miesto kde patri ptr
            // aby som neskor nemusel sortovat pointre
            // takto zabezpecim ze budu v pamati vzdy za sebou a collector bude 
            // mat jednoduchsiu pracu
        	if(pom > blok){
                
				// nastavenie pointrov
        		pred->dalsi = blok;
        		blok->dalsi = pom;
        		break;              // Dalej uz nemusime ist a mozeme to ukoncit
        	}
        	pred	= pom;
        	pom 	= pom->dalsi;
        }
        
        // Nastavenie velkosti a priznaku
        blok->velkost    = *((unsigned short *)ptr) + sizeof(unsigned short);
        blok->jeVolny    = TRUE;
    }
    // V pripade NULL poitra vypiseme hlasku a ukoncime program
    else {
        printf("Invalid pointer!");
        exit(1);
    }
}

void memory_init(void *ptr, unsigned int size) {
    
    META * prvyVolny = (ptr) + sizeof(META);
    
    // Hlavicka
    zvp = (META *) ptr;
    zvp->dalsi = prvyVolny;
    
    // Prva meta v pamati
    prvyVolny->jeVolny  = 1;
    prvyVolny->velkost  = size - sizeof(META);  // Velkost musime znizit o velkost hlavicky
    prvyVolny->dalsi    = NULL;                 // Dalsi je zatial NULL
}

// Funkcia vykona permutacie prvkov v danom poli
// v rozmedzi zac to koniec
void backtrack_paths(int *obchody, int from, int to) {
    
    int i;
    int dlzka = 0;
    
    // Permutujeme prvky v rozmedzi zac,koniec
    if(from != to) {
        
        for(i = from; i <= to; i++){
            x_swap((obchody + from),(obchody + i));
            backtrack_paths(obchody, from +1, to);
            x_swap((obchody - from),(obchody - i));
        }
    }
    // Ratanie dlzky cesty z domu cez vsetky obchody
    // naspat domov
    else {
        for (i = 0; i <= to + 1; i++) {
//        	printf("%d ", obchody[i]);
            // Nemusime ratat dalej ked nam to uz nevyhovuje
            if(dlzka > cesta)
                break;
            dlzka += d2[obchody[i]][obchody[i + 1]];
        }
//        printf("\n");
        // Ked mame kratsiu cestu
        // to je zatial nasa najkratsia
        if(dlzka < cesta)
            cesta = dlzka;
    }
}

// Pomocna funkcia k fukncii perm()
// zamiena hodnoty danych prvkov
// pomocou operacie bitovej xor
void x_swap(int *i, int *j) {
    
    if (i != j) {
        *i ^= *j;
        *j ^= *i;
        *i ^= *j;
    }
}

// Funkcia zisti najblizsie cesty z riadku s na matici
void dijkstra(int s,int size) {
    
    // pomocne premenne
	int i, j;
    int minimum; // minimum
    // pomocne pole navctivenych
	int * navst = memory_alloc((size + 1) * sizeof(int));
//    d = calloc(size + 1, sizeof(int));
    
	for (i = 0; i < size; i++) {
		d[i] = NEKONECNO; // inicializacia riadku matice na Nekonecno
		navst[i] = 0;
	}
    d[s] = 0;
	for (j = 0; j < size; j++) {
		minimum = -1;
		for (i = 0; i < size; i++) {
			if (((minimum == -1) || (d[i] < d[minimum])) && (navst[i] == 0)){
				minimum = i;
            }
        }
		navst[minimum] = 1;
		for (i = 0; i < size; i++){
			if (mat[minimum][i]){
				if (d[minimum] + mat[minimum][i] < d[i]) {
					d[i] = d[minimum] + mat[minimum][i];
                }
            }
        }
    }
}

