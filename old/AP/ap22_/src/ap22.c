#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define SIZE 10

int main(void){

	int i;
	char * str = NULL;
	char * ptr = NULL;

	if((str = (char *) malloc(SIZE)) == NULL)
		exit(-1);

	srand(time(NULL));

	for(i = 0; i < SIZE; i++){				// naplnim pole znakmi A - Z
		str[i] = rand()%(90 - 65) + 65;
		printf("%c ", str[i]);
	}
	printf("\n");

	ptr = str;
	printf("%p\n", ptr);					// vypisem adresu pointra
	ptr += 1; 								// pripocitam 1
	*ptr = 65;								// zmenim hodnotu na 65 (A)
	printf("%p %c\n", ptr, *ptr); 			// vypisem adresu a hodnotu pointra
	ptr += rand()%1000; 					// pointru priradim nahodnu adresu o 0-999 vacsiu ako povodna
	printf("%p %s\n", ptr, *ptr); 			// vypisem, pointer nemusi existovat. napr adresa mimo rozsahu pamete

	free(str);

	return 0;
}
