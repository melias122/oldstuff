#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char *binary(int c){

	char *b = NULL, *bin = NULL;
	unsigned int i, j;

	if((b = (char *)malloc(32)) == NULL){
		printf("Nepodarilo sa alokovat pamat\n");
		exit(-1);
	}

	i = 0;
	while(1){
		if(c % 2 == 0)
			b[i] = '0';
		else
			b[i] = '1';

		if((c /= 2) == 0){
			break;
		}
		i++;
	}

	if((bin = (char *) malloc(strlen(b) + 1)) == NULL){
		printf("Nepodarilo sa alokovat pamat\n");
		exit(-1);
	}

	for(j = i, i = 0; j > 0; j--, i++){
		bin[i] = b[j];
	}
	bin[i+1] = '\0';

	free(b);

	return bin;
}

int main(void) {

	char z;
	unsigned int c = 0;

	printf("Zadaj cislo a znak: ");
	scanf("%d %c", &c, &z);

	printf("(%d) 2: %s, 10: %d, 16: %X \n(%c) 2: %s, 10: %d, 16: %X \n", c, binary(c), c, c, z, binary((int)z), z, z);

	return 0;
}
