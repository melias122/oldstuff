#include <stdio.h>

//static int iCislo1, iCislo2, iVysledok;         // Niektore verzie potrebuju globalne premenne
static char str1[12], str2[12], str3[12];

int main(int argc, char* argv[]){

#ifdef __GNUC__                      // Tato cast sa preklada len v Dev-C++ (gcc)

   asm(".intel_syntax noprefix 	\n"   // Prepneme z AT&T syntaxe na na Intel

		   "mov eax, 0 \n"
		   "cpuid \n"
		   "mov str1, ebx \n"
		   "mov str2, ecx \n"
		   "mov str3, edx \n"

       ".att_syntax         \n"); // Dame vsetko do povodneho stavu

#elif _MSC_VER                     	// Tato cast sa preklada iba v MS Visual C++

    __asm {                        	// zaciatok bloku asm
	   MOV EAX, 0
	   CPUID
	   MOV str1, EBX
	   MOV str2, ECX
	   MOV str3, EDX
   }

#endif

    printf("%s%s%s\n", str1, str3, str2);

    return(0);
}
