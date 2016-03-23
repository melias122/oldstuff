#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void intel_hex_sum(char * str){

	unsigned int i, j, sum = 0, pom = 0;
	char hex[2];

	for(i = 1; i < strlen(str) - 2; i += 2){
		for(j = 0; j < 2; j++){
			hex[j] = str[i + j];
		}
		sscanf(hex, "%x", &pom);
		sum += pom;
//		printf("%X ", pom);
	}
	printf("SUM : %X ", (~sum%256) + 1);
}

int main(void) {

	char *str = ":10010000214601360121470136007EFE09D2190140";
//	char *str = ":100110002146017EB7C20001FF5F16002148011988";
//	char *str = ":10012000194E79234623965778239EDA3F01B2CAA7";
//	char *str = ":100130003F0156702B5E712B722B732146013421C7";
//	char *str = ":00000001FF";
	intel_hex_sum(str);

	return EXIT_SUCCESS;
}
