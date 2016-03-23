#include <stdio.h>

static int iCislo1, iVysledok;         // Niektore verzie potrebuju globalne premenne

int main(int argc, char* argv[]){

    iVysledok = 0;

	printf("Zadaj cislo (0 - 15): ");
	scanf("%d", &iCislo1);

#ifdef __GNUC__                      // Tato cast sa preklada len v Dev-C++ (gcc)

   asm(	".intel_syntax noprefix \n"   // Prepneme z AT&T syntaxe na na Intel

		   "mov eax, iCislo1 \n" 	// iCislo1 -> EAX
		   "sub eax, 9	\n"		// EAX + iCislo2
		   "mov ecx, 0	\n"
		   "cmp eax, ecx \n"
		   "jg skok \n"

		   "mov eax, iCislo1 \n"
		   "add eax, 48	\n"
		   "jmp end	\n"

		   "skok: \n"
		   "mov eax, iCislo1 \n"
		   "add eax, 55	\n"

		   "end: \n"
		   "mov iVysledok, eax \n"   // EAX  -> iVysledok

		   ".att_syntax \n"); // Dame vsetko do povodneho stavu

#elif _MSC_VER                     	// Tato cast sa preklada iba v MS Visual C++

    __asm {                        	// zaciatok bloku asm

	   MOV eax, iCislo1
	   SUB eax, 9
	   MOV ecx, 0
	   CMP eax, ecx
	   JG skok

	   MOV eax, iCislo1
	   ADD eax, 48
	   JMP end

	   skok:
	   MOV eax, iCislo1
	   ADD eax, 55

	   end:
	   MOV iVysledok, eax
    }                              	// koniec bloku asm

#endif

    printf("\n%d = %c", iCislo1, (char)iVysledok);

    return(0);
}
