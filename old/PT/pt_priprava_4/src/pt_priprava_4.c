#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define SIZE 8

int sucet2(int* pole_begin, int* pole_end){

	if(pole_begin != pole_end){
		return *pole_begin + sucet2(pole_begin + 1,pole_end);
	}
	return *pole_begin;
}

int sucet(int* pole, int size){

	int suma_left = 0, suma_right = 0;

	if( size == 1 )
		return *pole;

	suma_left  =  sucet(pole, size/2);
	suma_right =  sucet(pole + size/2, (size%2 == 0) ? size/2 : size/2 + 1);
	return suma_left + suma_right;
}

void print(int *arr, int length){
	int i;
	for(i = 0; i < length; ++i){
		printf("%d ", arr[i]);
	}
	printf("\n");
}

int rozklad(int n){

	return 0;
}

int *rand_arr(int size, int a, int b){

	int *arr = (int *) malloc(size * sizeof(int));

	while(size-- > 0)
		arr[size] = rand()%(b - a + 1) + a;

	return arr;
}

void swap(int *a, int *b){
	int tmp = *a;
	*a = *b;
	*b = tmp;
}

void merge(int *p1, int p1_size, int *p2, int p2_size){

	int i, p1_index = 0, p2_index = 0, tmp;


	for(i = 0; i < p1_size + p2_size; i++){
//		if(p1_index == p1_size){
//
//		}
//		if(p2_index == p2_size){
//
//		}

		if(p1[p1_index] <= p2[p2_index]){
			p1_index++;
		}
		else{
			swap(&p1[p1_index], &p2[p2_index]);
			p2_index++;
		}
	}
}
// 1 1 3 2 3
void merge_sort(int *pole, int size){

	if(size <= 1)
		return;

	merge_sort(pole, size/2);
	merge_sort(pole + size/2, (size%2 == 0) ? size/2 : size/2 + 1);

	merge(pole, size/2, pole + size/2, (size%2 == 0) ? size/2 : size/2 + 1);
}

int **alloc(int n, int m){

	int i;
	int **p = (int **) malloc(n * sizeof(int *));

	for(i = 0; i < n; i++)
		p[i] = (int *) malloc(m * sizeof(int));

	return p;
}

void rand_arr2(int **p, int n, int m, float per){
	int i, j;

	for(i = 0; i < n; i++){
		for(j = 0; j < m; j++){
			p[i][j] = rand()%2;
		}
	}
}

void print2(int **p, int n, int m){
	int i, j;
	for(i = 0; i < n; i++){
		for(j = 0; j < m; j++){
			printf("%d ",p[i][j]);
		}
		printf("\n");
	}
}

int main(void) {

	int * pole = NULL, **pole2 = NULL;

	srand(time(NULL));

	pole = rand_arr(SIZE, 1, 4);
	print(pole, SIZE);
	merge_sort(pole, SIZE);
	print(pole, SIZE);

	printf("%d\n", sucet(pole, SIZE));
	printf("%d\n", sucet2(pole, pole + SIZE - 1));

	pole2 = alloc(5,6);
	rand_arr2(pole2, 5, 6, 0.1);
	print2(pole2, 5, 6);


	return EXIT_SUCCESS;
}
