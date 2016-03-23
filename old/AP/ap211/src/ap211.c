#include <stdio.h>

static int iCislo1, iCislo2, iVysledok;         // Niektore verzie potrebuju globalne premenne

int main(int argc, char* argv[]){

    iVysledok = 0;

	printf("Zadaj dve cisla: ");
	scanf("%d %d", &iCislo1, &iCislo2);

#ifdef __GNUC__                      // Tato cast sa preklada len v Dev-C++ (gcc)

   asm(".intel_syntax noprefix 	\n"   // Prepneme z AT&T syntaxe na na Intel

       "mov eax, iCislo1    \n"   // iCislo1 -> EAX
       "add eax, iCislo2 	\n"   // EAX + iCislo2
       "mov iVysledok,eax  	\n"   // EAX  -> iVysledok

       ".att_syntax         \n"); // Dame vsetko do povodneho stavu

#elif _MSC_VER                     	// Tato cast sa preklada iba v MS Visual C++

    __asm {                        	// zaciatok bloku asm
    MOV EAX, iCislo1				// do EAX vloz hodnotu premennej iCislo1 (z pamate)
    ADD EAX, iCislo2 				// EAX + iCislo2
    MOV iVysledok,EAX              	// do premennej iVysledok vloz vysledok z registra EAX
    }                              	// koniec bloku asm

#endif

    printf("\n%d + %d = %d", iCislo1, iCislo2, iVysledok);

    return(0);
}
