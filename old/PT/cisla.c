#include <stdio.h>
#include <stdlib.h>


#define MAXLEN 10000
#define MAX(a, b) a > b ? a : b


void reverse(char s[]);
void scitaj(char *c1, char *c2);
//void nasob(int cislo1, int cislo2);
//void umocni(int cislo1, int cislo2);

void reverse(char s[])
{
      int c, i, j;

      for (i = 0, j = strlen(s)-1; i < j; i++, j--) {
         c = s[i];
         s[i] = s[j];
         s[j] = c;
      }
}

void scitaj(char *c1, char *c2){

	int c1_len = 0, c2_len = 0, max = 0, i = 0, n1 = 0, n2 = 0, spolu = 0, zvysok = 0;
	char res[MAXLEN];

	reverse(c1);
	reverse(c2);

	c1_len = strlen(c1);
	c2_len = strlen(c2);

	max = MAX(c1_len, c2_len);


	while(i <= max){

		if(i < c1_len)
			n1 = ((int)c1[i] - 48);
		else
			n1 = 0;

		if(i < c2_len)
			n2 = ((int)c2[i] - 48);
		else
			n2 = 0;



		if((spolu = n1 + n2 + zvysok) > 9){
			spolu %= 10;
			zvysok = 1;
		}
		else
			zvysok = 0;

		res[i] = (char)(spolu + 48);
		i++;
	}
	res[i] = '\0';
	reverse(res);
	printf("%s\n", res);
}

int main(void) {

//	char cislo1[] = "000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000\0"; 	//max velkost cisla najviac 100 cislic
//	char cislo2[] = "000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000\0";

	char * op;			//operator 1 znak
	char * cislo1;
	char * cislo2;

	if((op = (char *) malloc(2 * sizeof(char))) == NULL){
		printf("cant malloc mem\n");
		return -1;
	}
	if((cislo1 = (char *) malloc(101 * sizeof(char))) == NULL){
		printf("cant malloc mem\n");
		return -1;
	}
	if((cislo2 = (char *) malloc(101 * sizeof(char))) == NULL){
		printf("cant malloc mem\n");
		return -1;
	}


	while(scanf("%s %s %s", cislo1, op, cislo2) > 0){

		if(op[0] == '+')
			scitaj(cislo1, cislo2);
//		if(op == '*')
//			nasob(cislo1, cislo2);
//		if(op == '^')
//			umocni(cislo1, cislo2);
		}

	return 0;
}
