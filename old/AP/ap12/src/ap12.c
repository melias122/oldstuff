#include <stdio.h>
#include <stdlib.h>

//I = 1
//V = 5
//X = 10
//L = 50
//C = 100
//D = 500
//M = 1000

char *rimsky(int c){

	char *buf;
	int i = 0;

	if((buf = (char *) malloc(1024)) == NULL){
		exit(-1);
	}

	while(c != 0){
		if(c >= 1000){
			buf[i] = 'M';
			c -= 1000;
			goto it1;
		}
		else if(c >= 900){
			buf[i] = 'C'; buf[i+1] = 'M';
			c -= 900;
			goto it2;
		}
		else if(c >= 500){
			buf[i] = 'D';
			c -= 500;
			goto it1;
		}
		else if(c >= 400){
			buf[i] = 'C'; buf[i+1] = 'D';
			c -= 400;
			goto it2;
		}
		else if(c >= 100){
			buf[i] = 'C';
			c -= 100;
			goto it1;
		}
		else if(c >= 90){
			buf[i] = 'X'; buf[i+1] = 'C';
			c -= 90;
			goto it2;
		}
		else if(c >= 50){
			buf[i] = 'L';
			c -= 50;
			goto it1;
		}
		else if(c >= 40){
			buf[i] = 'X'; buf[i+1] = 'L';
			c -= 40;
			goto it2;
		}
		else if(c >= 10){
			buf[i] = 'X';
			c -= 10;
			goto it1;
		}
		else if(c >= 9){
			buf[i] = 'I'; buf[i+1] = 'X';
			c -= 9;
			goto it2;
		}
		else if(c >= 5){
			buf[i] = 'V';
			c -= 5;
			goto it1;
		}
		else if(c >= 4){
			buf[i] = 'I'; buf[i+1] = 'V';
			c -= 4;
			goto it2;
		}
		else if(c >= 1){
			buf[i] = 'I';
			c -= 1;
			goto it1;
		}
		it1:
			i++;
			continue;
		it2:
			i += 2;
			continue;
	}
	buf[i+1] = '\0';

	return buf;
}

int main(void) {

	int c = 0;

	printf("Zadaj nezaporne cele cislo: ");
	scanf("%d", &c);

	if(c > 0)
		printf("Cislo %d rimsky: '%s'\n", c, rimsky(c));
	else
		printf("Cislo musi byt vacsie ako 0\n");

	return 0;
}
