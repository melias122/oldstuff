#include <stdio.h>
#include <stdlib.h>

void swap1(int a, int b ){
  int c;
  //Uloha 1. doplnte kod ktory vemeni obsah premennych a,b
  c = b;
  b = a;
  a = c;
}

void swap2(int* a, int* b ){
   int c;
   //Uloha 2. dorobte swap2 aby sa vemenili obsahy premennych na adresach ulozenych v a,b
   c = *b;
   *b = *a;
   *a = c;
}

void tlac_pole(int* zaciatok, int velkost){
	int i;
	for(i = 0; i < velkost; i++){
		printf("%d ", *(zaciatok + i));
	}
	printf("\n");
}

void napln_pole(int* zaciatok, int velkost){
	int i;
	for(i = 0; i < velkost; i++){
		*(zaciatok + i) = rand()%91 + 10;
	}
}

void bubble_sort(int *p, int velkost){
	int i, j;

	for(i = 1; i <= velkost; i++){
		for(j = 0; j < velkost - i; j++){
			if( p[j] > p[j+1] ){
				swap2(&p[j], &p[j+1]);
			}
		}
	}
}

void insertion_sort(int *p, int velkost){
	int i, j, pom;

	for(i = 1; i < velkost; i++){
		pom = p[i];
		for (j = i; j > 0; j--) {
			if(pom < p[j-1]){
				p[j] = p[j-1];
				p[j-1] = pom;
			}
			else
				break;
		}
	}
}

void selection_sort(int *p, int velkost){
	int i, j, *pom = NULL;

	for(i = 0; i < velkost; i++){
		pom = &p[i];
		for(j = i+1; j < velkost; j++){
			if(p[j] <= *pom){
				pom = &p[j];
			}
		}
		swap2(&p[i], pom);
	}
}

#define VELKOST 100

int main(void){

  int *pole;
  int e = 1, f = 2;

  if((pole = (int *) malloc(VELKOST * sizeof(int))) == NULL){
	  return -1;
  }

   swap1(e,f); //Uloha 1.
//   printf("%d %d\n", e, f);
   swap2(&e,&f); //Uloha 2.
//   printf("%d %d\n", e, f);
   napln_pole(pole, VELKOST);
   tlac_pole(pole, VELKOST);
//   tlac_pole(pole+10, 10);
//   bubble_sort(pole, VELKOST);
//   insertion_sort(pole, VELKOST);
   selection_sort(pole, VELKOST);
   tlac_pole(pole, VELKOST);

   free(pole);

   return 0;
}
