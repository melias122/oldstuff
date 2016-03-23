#include <stdio.h>
#include <stdlib.h>

int main(void) {

	char file_name[] = "koniec.htm";
	char file_name_new[] = "koniec_new.htm";
	FILE *file = NULL, *file_new = NULL;

	if((file = fopen(file_name, "rb")) == NULL){
		printf("Subor %s sa neda otvorit!\n", file_name);
		exit(-1);
	}
	if((file_new = fopen(file_name_new, "w")) == NULL){
		printf("Subor %s sa neda otvorit!\n", file_name_new);
		exit(-1);
	}

	unsigned int c = 0;
	while ((c = getc(file)) != EOF){
		c ^= 27;
		putc(c, file_new);
	}

	fclose(file);
	fclose(file_new);

	printf("OK!\n");

	return EXIT_SUCCESS;
}
