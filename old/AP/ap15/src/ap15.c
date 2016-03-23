#include <stdio.h>
#include <stdlib.h>

void spaces(int c){
	int i;
	for(i = 0; i < c; i++)
		printf(" ");
}

void pascal_t(int depth){
	int i, j, k, *akt, *pred;

	if((akt = (int *) malloc(depth * sizeof(int))) == NULL)
		exit(-1);
	if((pred = (int *) malloc(depth * sizeof(int))) == NULL)
		exit(-1);

	for(i = 0; i < depth; i++){

		spaces(3*(depth - i - 1));

		if(i == 0){
			printf("1\n");
			continue;
		}

		for(j = 0; j < i+1; j++){

			if(j == 0){ // lava strana
				akt[j] = 1;
				printf("1");
				spaces(5);
				continue;
			}
			else if(j == i){ // prava strana
				akt[j] = 1;
				printf("1\n");

				for(k = 0; k < i + 1; k++){
					pred[k] = akt[k];
				}
				continue;
			}
			akt[j] = pred[j -1] + pred[j];
			if( (akt[j] % 2) == 0){ // stred parne
				printf("*");
				spaces(5);
			}
			else{ // stred neparne
				printf("%d", akt[j]);
				if(akt[j] < 10){
					spaces(5);
					continue;
				}
				if(akt[j] < 100){
					spaces(4);
					continue;
				}
				if(akt[j] < 1000){
					spaces(3);
					continue;
				}
				else{
					spaces(2);
				}
			}
		}
	}
	free(akt);
	free(pred);
}

int main(void) {
	int n = 0;

	printf("zadaj velkost trojuholnika: ");
	scanf("%d", &n);

	pascal_t(n);

	return 0;
}
