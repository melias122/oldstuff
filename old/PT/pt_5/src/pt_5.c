#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void print(int *pole, int size){
	int i;
	for(i = 0; i < size; i++)
		printf("%d ", pole[i]);
	printf("\n");
}

int *alloc1D(int n){
	return (int *) malloc(n * sizeof(int));
}

void pridaj_do_pola(int *pole, int pocet, int index){

	if(index == 0)
		pole[index] = 0;
	else if(index < pocet)
		pole[index] = pole[index -1] + index;
	else
		return;

	pridaj_do_pola(pole, pocet, ++index);
}

int maximum(int *pole, int size){

	if( size == 1 )
		return *pole;

	int left_max  =  maximum(pole, size/2);
	int right_max =  maximum(pole + size/2, (size%2 == 0) ? size/2 : size/2 + 1);

	if(left_max < right_max)
		return right_max;
	else
		return left_max;
}

//int sucet(int *pole, int from, int to){
//
//	if(from <= to)
//		return pole[from] + sucet(pole, from + 1, to);
//	else
//		return 0;
//}

int sucet(int* pole, int size){

	int suma_left = 0, suma_right = 0;

	if( size == 1 )
		return *pole;

	suma_left  =  sucet(pole, size/2);
	suma_right =  sucet(pole + size/2, (size%2 == 0) ? size/2 : size/2 + 1);
	return suma_left + suma_right;
}

int max_predchodca(int *pole, int size, int index){

	int i_max;

	if(index < size)
		i_max = max_predchodca(pole, size, ++index);
	else
		return index - 1;

	if(index == 1){
		if((pole[i_max - 1] > pole[index - 1]))
			return pole[i_max];
		else
			return pole[index];
	}
	else{
		if((pole[i_max - 1] > pole[index - 1]))
			return i_max;
		else
			return index;
	}
}

int main(void) {

	int * pole = NULL;
	int size, i;

	srand(time(NULL));

	size = 11;

	pole = alloc1D(size);
	pridaj_do_pola(pole, size, 0);
	printf("pole1: ");
	print(pole, size);

	free(pole);
	pole = alloc1D(size);
//	int sum = 0;
	for(i = 0; i < size; i++){
		pole[i] = -20 + rand()%41;
//		sum += pole[i];

	}
//	printf("sum: %d\n", sum);
	printf("pole2: ");
	print(pole, size);
	printf("maximum: %d\n", maximum(pole, size));
	printf("sucet: %d\n", sucet(pole, size - 1));
	printf("max predchodca: %d\n", max_predchodca(pole, size, 0));


	return EXIT_SUCCESS;
}
