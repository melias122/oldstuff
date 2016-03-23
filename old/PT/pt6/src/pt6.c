
//Vasou ulohou je:
//1. Opravit quick_sort a otestovat ho na poli (4b)
//2. Dorobit funkciu generuj_mnozinu() a print_mnozina() pre strukturu mnozina (2b)
//3. Dorobit funckiu porovnaj_mnoziny(), ktora porovna mnoziny(2b)
//4. Vygenerovat zretazeny zoznam mnozin, ktore su usporiadane (4b)

#include<stdio.h>
#include<stdlib.h>
#include <time.h>

void odriadkuj(){printf("\n");}

int* rand_pole(int size, int bound){
        int * pole = (int*)malloc(sizeof(int)*size);

        int i;
        for(i = 0; i < size; i++)
        {
                pole[i] = rand() % bound;
        }

        return pole;
}
void print_pole(int *pole, int size){
	int i;
	for(i = 0; i < size; i++)
        {
                printf("%i ", pole[i]);
        }
        printf("\n");
}
void swap(int *a, int *b)
{
        int c;
        c = *a;
        *a = *b;
        *b = c;
}
// spracuvam interval <left,right>, pivota vyberam ako najpravejsi prvok (index  right-1)
int partition(int *pole, int left, int right){
        int i,save_index = left,pivot;
        pivot = pole[right - 1];

//        swap(pole[save_index],pole[right]);
        for(i = left; i < right ; i++){
                if( pole[i] < pivot ){
//                	swap(pole + i, pole + save_index);
                	swap(&pole[save_index], &pole[i]);
                	save_index++;
                }
        }
        swap(pole+save_index,&pole[right -1]);

        return save_index;
}
void quick_sort(int *pole, int left, int right){
        if(left >= right)return;
        int pivot_index = partition(pole,left,right);
//        print_pole(pole, right);

        quick_sort(pole,left,pivot_index -1);
        quick_sort(pole,pivot_index + 1,right);

}

struct Mnozina{
        int velkost;
        int* pole;
        struct Mnozina* next;
};

//vygeneruj mnozinu, utried jej prvky a vrat na nu pointer
struct Mnozina* generuj_mnozinu(int velkost)
{
	if(velkost < 1)
		return NULL;
	int i;
	struct Mnozina *new = (struct Mnozina *) malloc(sizeof(struct Mnozina));
	new->next = NULL;
	new->velkost = velkost;
	new->pole = (int *) malloc(velkost * sizeof(int));
	for(i = 0; i < velkost; i++){
		new->pole[i] = rand()%20;
	}
	return new;
}
//vytlac mnozinu
void print_mnozina(struct Mnozina* s){
	if(s == NULL)
		return;
	int i;
	for(i = 0; i < s->velkost; i++){
		printf("%d ", s->pole[i]);
	}
        odriadkuj();
}


//porovnaj mnoziny - vrat 0 alebo 1
// {2 1}<{2,1,2}<{2,2} - vrati 0 ak prva je "mensia"
int porovnaj_mnoziny(struct Mnozina *a, struct Mnozina *b){
	int i, velkost;

//	velkost = (a->velkost > b->velkost) ? b->velkost : a->velkost;

	if(a->velkost > b->velkost){
		velkost = b->velkost;
	}
	else{
		velkost = a->velkost;
	}

	for(i = 0; i < velkost; i++){
		if(a->pole[i] < b->pole[i])
			return 0;
	}

	return 1;
}


//korektna funkcia ntreba robit nic
void print_mnoziny(struct Mnozina* begin){
        struct Mnozina *temp = begin;
        while(temp != NULL)
        {
                print_mnozina(temp);
                temp = temp->next;
        }
        odriadkuj();
}


//vygeneruj zoznam mnozin aby zoznam bol utriedeny

struct Mnozina* generuj_mnoziny(int pocet)
{
        struct Mnozina *mnoziny,*nova_mnozina,*temp;

        //vygenerovanie prvej mnoziny
        mnoziny = generuj_mnozinu(rand()%20 + 1);

        //dopln zvysne mnoziny
        int i;
        for(i = 1; i < pocet; i++)
        {
        	temp = mnoziny;
                //vygeneruj novu mnozinu
                nova_mnozina = generuj_mnozinu(rand()%20 + 1);
//                quick_sort(nova_mnozina->pole,0, nova_mnozina->velkost);

                while(porovnaj_mnoziny(nova_mnozina,temp)){
                	if(temp->next == NULL)
                		break;
                	temp = temp->next;
                }
                if(temp == mnoziny){
                	nova_mnozina->next = mnoziny;
                	mnoziny = nova_mnozina;
                }
                else{
                	temp->next = nova_mnozina;
                	nova_mnozina->next = temp;
                }

                //vloz ju do zretazeneho zoznamu na svoje miesto
                //...


        }

        return mnoziny;
}

void main(){
        int *pole;

        srand(time(NULL));

        pole = rand_pole(20,15);
        printf("Nahodne pole:\n");
        print_pole(pole,20);


        //utried a vytlac
        quick_sort(pole,0,20);
        printf("\nUtriedene pole:\n");
        print_pole(pole,20);


        //Korektne utriedene 20 tich prvkov  == 4b


        struct Mnozina  *mnozina1 = generuj_mnozinu(1),
                                        *mnozina2 = generuj_mnozinu(3),
                                        *mnozina3 = generuj_mnozinu(5),
                                        *mnozina4 = generuj_mnozinu(0);
        //korektne vygenerovane mnoziny dlzky 1,3,5,0           =1b

        printf("\n\nNahodne retazce:");
        printf("\nMnozina dlzky 1:"); print_mnozina(mnozina1);
        printf("\nMnozina dlzky 3:"); print_mnozina(mnozina2);
        printf("\nMnozina dlzky 5:"); print_mnozina(mnozina3);
        printf("\nMnozina dlzky 0:"); print_mnozina(mnozina4);
        //korektne vytlacene nahodne mnoziny dlzky 1,3,5,0      =1b

        printf("\n\nPorovnanie mnozin:\n");
        printf("1. vs 2. = %i \n", porovnaj_mnoziny(mnozina1,mnozina2));
        printf("1. vs 3. = %i \n", porovnaj_mnoziny(mnozina1,mnozina3));
        printf("2. vs 3. = %i \n", porovnaj_mnoziny(mnozina2,mnozina3));
        //korektne porovnane mnoziny 1b

        printf("\n\nNahodne mnoziny usporiadane: \n");
        print_mnoziny(generuj_mnoziny(10));
        // korektne utriedenych 10 mnozin  =4b

}
