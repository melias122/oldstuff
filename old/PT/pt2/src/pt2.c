#include <stdio.h>
#include <stdlib.h>

int * alokuj(int n){
	return (int *) malloc(n * sizeof(int));
}

void swap(int * ptr1, int * ptr2 ){

	int c;

   c = *ptr2;
   *ptr2 = *ptr1;
   *ptr1 = c;
}

void vypis(int *pole, int size){
	int i;

	for(i = 0; i < size; i++){
		printf("%d ", pole[i]);
	}
	printf("\n");
}

void bubble_sort(int *p, int velkost){
	int i, j;

	for(i = 1; i <= velkost; i++){
		for(j = 0; j < velkost - i; j++){
			if( p[j] > p[j+1] ){
				swap(&p[j], &p[j+1]);
			}
		}
	}
}

void usporiadaj(int *pole1, int *pole2, int n1, int n2){

	int * pole;
	int i;

	pole = alokuj(n1+n2);

	for(i = 0; i < n1; i++){
		pole[i] = pole1[i];
	}
	for(i = 0; i < n2; i++){
		pole[i+n1] = pole2[i];
	}

	bubble_sort(pole, n1+n2);

	for(i = 0; i < n1; i++){
		pole1[i] = pole[i];
	}
	for(i = 0; i < n2; i++){
		pole2[i] = pole1[i+n1];
	}
}

int napln(int **pole1, int **pole2, FILE *f){

	int size = 0;

	if((*pole1 = (int *) malloc(sizeof(int))) == NULL){
		printf("Nepodarilo sa alokovat pamat\n");
		exit(-1);
	}
	if((*pole2 = (int *) malloc(sizeof(int))) == NULL){
		printf("Nepodarilo sa alokovat pamat\n");
		exit(-1);
	}

	return size;
}

void ttest(int *p){

	int * p_new;
	if((p_new = (int *) malloc(2 * sizeof(int))) == NULL){
		printf("Nepodarilo sa alokovat pamat\n");
		exit(-1);
	}
	*p_new = 1;
	p = &p_new;
}

int main(void) {

	int **p1 = NULL, *p2 = NULL;

	FILE *s;

	if((s = fopen("subor.txt", "r")) == NULL){
		printf("nepodarilo sa otvorit subor\n");
		exit(-1);
	}

	p1 = (int **) malloc(sizeof(int *));
//	p2 = (int *) malloc(sizeof(int *));

	napln(p1, p2,s);

	printf("%p %p %d\n", &p2, p2, *p2);
	ttest(&p2);
//	printf("%p %p %d", &p2, p2, *p2);

	fclose(s);

	return 0;
}
