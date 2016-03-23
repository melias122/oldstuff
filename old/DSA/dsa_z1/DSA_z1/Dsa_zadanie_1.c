#include <stdio.h>
#include <stdlib.h>

// Macro

#define FALSE 0
#define TRUE 1


// Struct

typedef struct _META {
    unsigned short jeVolny;     // priznak
    unsigned int velkost;       // velkost bloku
    struct _META * dalsi;       // pointer na dalsi volny
} META;

// Prototypy funkcii

void memory_free(void *ptr);
void *memory_alloc(unsigned int size);
void memory_init(void *ptr, unsigned int size);
void collect(void);

// Globalne premenne

META * zvp;

// Funkcie

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

int main(){
    return 0;
}


//int _test(int seed, int size, int maxAllocSize, int *memoryLevel, double minSuccessRate){
//    
//    int i, j, tmp, k = 0, b, pseudoCurrent = 0, current = 0, maxPtrCount = size*5, successAllocs = 0, failedAllocs = 0;
//    char *data = (char *)malloc(maxPtrCount);
//    int *length = (int *)malloc(maxPtrCount*sizeof(int));
//    void **ptr = (void **)malloc(maxPtrCount*sizeof(void *));
//    int memoryReached[100];
//    int *activeBlocks = (int*)malloc(maxPtrCount*sizeof(int)), nActive = 0;
//    char *p;
//    int error = 0, inconsistent_data = 0, out_of_range = 0;
//    double rate = 0.0;
//    char *mainptr = (char*)malloc(size);
//    
//    printf("Vyhradeny blok: %d byte\n", size);
//    memory_init(mainptr, size);
//    printf("memory_init() OK");
//    
//    srand(seed);
//    
//    for (i = 0; memoryLevel[i] && !error; i++)
//    {
//        printf("\n\n");
//        memoryReached[i] = current;
//        if (memoryLevel[i] > current)
//        {
//            printf("Alokovat aspon %d byte:\n", memoryLevel[i] - current);
//            pseudoCurrent = current;
//            // alloc new blocks
//            while (pseudoCurrent < memoryLevel[i])
//            {
//                data[k] = rand()%256;
//                length[k] = 5 + rand()%maxAllocSize;
//                printf("alloc(%d):", length[k]);
//                ptr[k] = memory_alloc(length[k]);
//                pseudoCurrent += length[k];
//                if (ptr[k] == NULL)
//                {
//                    printf("NULL");
//                    // neuspesna alokacia
//                    failedAllocs++;
//                    //break;
//                }
//                else
//                {
//                    printf("OK");
//                    
//                    p = (char*)ptr[k];
//                    for (tmp = 0; tmp < length[k]; tmp++)
//                        *(p+tmp) = data[k];
//                    
//                    if ((char*)ptr[k] < (char*)mainptr || (char*)ptr[k] >= (char*)(mainptr+size))
//                    {
//                        error = 1;
//                        out_of_range = 1;
//                        break;
//                    }
//                    
//                    
//                    // uspesna alokacia
//                    successAllocs++;
//                    current += length[k];
//                    memoryReached[i] = current;
//                    activeBlocks[nActive++] = k;
//                    k++;
//                }
//            }
//        }
//        else
//            if (memoryLevel[i] < current)
//            {
//                printf("Uvolnit aspon %d byte:\n", current - memoryLevel[i]);
//                pseudoCurrent = current;
//                // free existing blocks
//                while (current > memoryLevel[i])
//                {
//                    j = rand()%nActive;
//                    b = activeBlocks[j];
//                    
//                    // dealokuj block j
//                    
//                    // skontroluj, ci su data konzistentne
//                    p = (char*)ptr[b];
//                    for (tmp = 0; tmp < length[b]; tmp++)
//                        if (*(p+tmp) != data[b])
//                            break;
//                    
//                    if (tmp < length[b])
//                    {
//                        error = 1;
//                        inconsistent_data = 1;
//                        break;
//                    }
//                    
//                    printf("free(%d):", length[b]);
//                    memory_free(ptr[b]);
//                    printf("OK ");
//                    
//                    current -= length[b];
//                    memoryReached[i] = current;
//                    
//                    activeBlocks[j] = activeBlocks[--nActive];
//                }
//            }
//    }
//    printf("\n");
//    
//    if (error) {
//        if (inconsistent_data)
//            printf("Chyba: Pridelujete prekryvajucu sa pamat (nejaky blok pamate ste pridelili viac krat.\n");
//        if (out_of_range)
//            printf("Chyba: Pridelili ste pamat mimo vyhradeneho bloku, ktory ste dostali do memory_init funkcie.\n");
//    }
//    if (failedAllocs)
//        printf("Chyba: Nezvladli ste alokovat vsetky pozadovane urovne pamate -- cim lepsie viete dosiahnut pozadovane urovne, tym lepsie.\n");
//    
//    printf("Pocet uspesnych alokacii: %d\n", successAllocs);
//    printf("Pocet neuspesnych alokacii: %d\n", failedAllocs);
//    
//    rate = ((double)successAllocs)/(double)(successAllocs+failedAllocs);
//    printf("Uspesnost: %lf\n", rate);
//    
//    if (rate < minSuccessRate){
//        printf("NESPLNILI STE POZIADAVKU NA USPESNOST ALOKACII.\n");
//        error = 1;
//    }
//    
//    if (error)
//        return 1;
//    return 0;
//}


// test 1
//                    int main(void) { int levels[] = {50, 20, 70, 30, 0}; if (_test(1, 200, 10, levels, 1.0)) return 1; return 0;};

// test 2
//                        int main(void) { int levels[] = {10, 5, 20, 5, 0}; if (_test(2, 50, 10, levels, 1.0)) return 1; return 0; };

// test 3
//                        int main(void) { int levels[] = {200, 50, 300, 50, 400, 50, 0}; if (_test(3, 500, 10, levels, 1.0)) return 1; return 0; };


