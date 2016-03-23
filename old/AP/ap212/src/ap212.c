#include <stdio.h>

static int iCislo1, iVysledok;         // Niektore verzie potrebuju globalne premenne

int main(int argc, char* argv[]){

    iVysledok = 0;

	printf("Zadaj cislo: ");
	scanf("%d", &iCislo1);

#ifdef __GNUC__                      // Tato cast sa preklada len v Dev-C++ (gcc)

   asm(".intel_syntax noprefix 	\n"   // Prepneme z AT&T syntaxe na na Intel

       "mov eax, iCislo1    \n"   // iCislo -> EAX
       "shl eax, 1 			\n"   // EAX << iCislo2
       "mov iVysledok,eax  	\n"   // EAX  -> iVysledok

       ".att_syntax         \n"); // Dame vsetko do povodneho stavu

#elif _MSC_VER                     	// Tato cast sa preklada iba v MS Visual C++

    __asm {                        	// zaciatok bloku asm
    MOV EAX, iCislo1				// do EAX vloz hodnotu premennej iCislo (z pamate)
    SHL EAX, 1		 				// shift 1
    MOV iVysledok,EAX              	// do premennej iVysledok vloz vysledok z registra EAX
    }                              	// koniec bloku asm

#endif

    printf("\n%d * 2 = %d", iCislo1, iVysledok);

    return(0);
}
