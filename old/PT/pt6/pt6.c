//Vasou ulohou je:
//1. Opravit min_sort a otestovat ho na zretazenom zozname (4b)
//2. Dorobit fciu generuj a vypis pre string (retazec reprezentovany ako struktura kde si pamatame aj jej dlzku a nasledujuci retazec)(2b)
//3. Dorobit funckiu ktora porovna stringy(2b)
//4. Vygenerovat zoznam retazcov a utriedit ich min_sortom(upravte si ho ako potrebujete) vytlacit (4b)

#include<stdio.h>
#include<stdlib.h>
#include<time.h>

void odriadkuj(){printf("\n");};

struct Item{
	struct Item *next;
	int value;
};

struct Item* rand_zoznam(int size, int bound){
	struct Item *begin = NULL, *new_item;
	int i;
	for(i = 0; i < size; i++)
	{
		new_item = (struct Item*)malloc(sizeof(struct Item));
		new_item->value = rand() % bound;
		new_item->next = begin;
		begin = new_item;
	}

	return begin;
}
void print_zoznam(struct Item *begin){
	struct Item* temp = begin;
	while(temp != NULL)
	{
		printf("%i ", temp->value);
		temp = temp->next;
	}
	printf("\n");
}

//najdi minimum zo zoznamu a vrat nan pointer
struct Item* find_min(struct Item *begin){
	struct Item *temp, *result = NULL;

	temp = result = begin;
	while(temp != NULL){
		if(temp->value < result->value ){
			result = temp;
		}
		temp = temp->next;
	}

	return result;
}

//najdi predchodcu k danemu prvku prvok
struct Item* find_previous(struct Item *begin, struct Item *prvok){
	struct Item *temp = begin;

	if(begin == prvok){
		return NULL;
	}

	while(temp->next != prvok){
		temp = temp->next;
	}

	return temp;
}

//pamatam si 2 zoznamy uz utriedne cisla(sorted) a neutriedene unsorted
//postupne pomocou funkcii find_min a find_previous pridam na koniec utriedenych najmensie cislo z neutriedenych
struct Item* min_sort(struct Item* begin){

	struct Item *unsorted,*sorted_end, *sorted = NULL, *minimum, *pred_min;

	unsorted = begin;
	sorted_end = sorted = find_min(unsorted);
	pred_min = find_previous(unsorted,sorted);
	if(pred_min)
		pred_min->next = pred_min->next->next;
	else
		unsorted = unsorted->next;

	while(unsorted != NULL){

		minimum = find_min(unsorted);
		if(minimum == unsorted){
			sorted_end->next = minimum;
			sorted_end = sorted_end->next;
			unsorted = unsorted->next;
		}
		else{
			pred_min = find_previous(unsorted,minimum);
			if(pred_min)
				pred_min->next = pred_min->next->next;
			sorted_end->next = minimum;
			sorted_end = sorted_end->next;

		}

	}
	return sorted;
}

struct string{
	char* znaky;
	int velkost;
	struct string* next;
};
//vygeneruj nahodny retazec(struktura)dlzky velkost a vrat nan pointer
struct string* generuj_string(int velkost)
{
	if(velkost < 1)
		return NULL;

	int i;

	struct string *s = (struct string *) malloc(sizeof(struct string));
	s->znaky = (char *) malloc(velkost * sizeof(char));
	s->velkost = velkost;
	s->next = NULL;

	for(i = 0; i < velkost; i++)
		s->znaky[i] = 65 + rand()%10;

	return s;
}
//vytlac retazec
void print_string(struct string* s){

	if(s){
		int i;
		for(i = 0; i < s->velkost; i++)
			printf("%c", s->znaky[i]);
	}
	odriadkuj();
}


//porovnaj retazce - vrat 0 alebo 1
// "ab" < "ba", "a"<"ab" - vrati 0 ak prva je prvy v slovniku pred druhym
int porovnaj_stringy(struct string *s1, struct string *s2){

	int i;

	for(i = 0; i < s1->velkost; i++){
		if(s1->znaky[i] == s2->znaky[i])
			continue;
		else if(s1->znaky[i] < s2->znaky[i])
			return 0;
		else
			return 1;
	}
	return 0;
}

//korektna funkcia ktora vrati zoznam stringov
struct string * generuj_stringy(int pocet){
	struct string *begin = NULL, *temp;
	int i;
	for(i = 0; i < pocet; i++){
		temp = generuj_string(5);
		temp->next = begin;
		begin = temp;
	}

	return begin;
}
// funkcia ktora ma vytlacit zoznam stringov
void print_stringy(struct string* begin){
	while(begin != NULL){
		print_string(begin);
		begin = begin->next;
	}
}

//funkciu min_sort2 na utriedenie
struct string* min_sort2(struct string *unsorted){
	struct string *sorted, *sorted_end, *min, *prev, *temp;

	while(unsorted){
		prev = NULL;
		temp = min = unsorted;
		while(temp){
			if(porovnaj_stringy(temp, min) == 0){
				min = temp;
			}
			prev = temp;
			temp = temp->next;
		}
		if(sorted == NULL){
			sorted = sorted_end = min;
		}
		else{
			sorted_end->next = min;
			sorted_end = sorted_end->next;
		}
		if(prev){
			prev->next = prev->next->next;
		}
		else{
			unsorted = unsorted->next;
		}
	}
	return sorted;
}


void main(){

	srand(time(NULL));

	struct Item *begin = rand_zoznam(10, 15);


	printf("Nahodny zoznam:\n");
	print_zoznam(begin);

	//utried a vytlac
	begin = min_sort(begin);
	printf("\nUtriedeny zoznam:\n");
	print_zoznam(begin);
	//Korektne utriedenie zoznamu celych cisel s 10timi prvkami		4b


	struct string  *s1 = generuj_string(1),
				   *s2 = generuj_string(3),
				   *s3 = generuj_string(5),
				   *s4 = generuj_string(0);
	//korektne vygenerovane retazce dlzky 1,3,5,0				=1b

	printf("\n\nNahodne retazce:");
	printf("\nRetazec dlzky 1:"); print_string(s1);
	printf("\nRetazec dlzky 3:"); print_string(s2);
	printf("\nRetazec dlzky 5:"); print_string(s3);
	printf("\nRetazec dlzky 0:"); print_string(s4);
	//korektne vytlacene nahodne retazce dlzky 1,3,5,0			=1b

	printf("\n\nPorovnanie retazcov:\n");
	printf("1. vs 2. = %i \n", porovnaj_stringy(s1,s2));
	printf("1. vs 3. = %i \n", porovnaj_stringy(s1,s3));
	printf("2. vs 3. = %i \n", porovnaj_stringy(s2,s3));
	//korektne porovnane retazce								=2b


	printf("\n\nNahodnych 10 retazcov dlzky 5: \n");
	struct string *S, *S_sorted;

	//odkomentuj dlasie 3 riadky ked budes mat dorobenu funkciu pre generovanie jedneho stringu a tlac stringu
	S = generuj_stringy(10);
	print_stringy(S);
	odriadkuj();


	//korektne vytlacenych 10  retazcov							=1b

	printf("\n\nUtriedenych 10 retazcov dlzky 5: \n");
	//odkomentuj ked budes mat dorobenu funkciu pre insert_sort2

	S_sorted = min_sort2(S);
	print_stringy(S_sorted);

	//korektne utriedenych 10  retazcov							=3b*/

}
