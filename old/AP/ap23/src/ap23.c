#include <stdio.h>
#include <stdlib.h>

int main(void){

	char file_name[] = "poviedka.htm";
	char file_name_new[] = "poviedka_new.htm";
	FILE *subor = NULL, *subor_new = NULL;

	if((subor = fopen(file_name, "r")) == NULL){
		printf("Subor %s sa neda otvorit!\n", file_name);
		exit(-1);
	}
	if((subor_new = fopen(file_name_new, "w")) == NULL){
		printf("Subor %s sa neda otvorit!\n", file_name_new);
		exit(-1);
	}

	unsigned int c = 0;
	while ((c = getc(subor)) != EOF){
		switch (c) {
			case 165: c = 188; break;
			case 169: c = 138; break;
			case 171: c = 141; break;
			case 174: c = 142; break;
			case 181: c = 190; break;
			case 185: c = 154; break;
			case 187: c = 157; break;
			case 190: c = 158; break;

			default:break;
		}
		putc(c, subor_new);
	}

	fclose(subor);
	fclose(subor_new);

	printf("OK!\n");

	return 0;
  }
/*ISO-8859-2   Windows-1250
  znak   hex  dec      hex  dec

   Ľ      A5  165       BC  188
   Š      A9  169       8A  138
   Ť      AB  171       8D  141
   Ž      AE  174       8E  142

   ľ      B5  181       BE  190
   š      B9  185       9A  154
   ť      BB  187       9D  157
   ž      BE  190       9E  158
   */
