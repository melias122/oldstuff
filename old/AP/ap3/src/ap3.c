#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <string.h>

HANDLE open_port(char * port){

	HANDLE hCom = CreateFile(
//				"COM3:",                         // Názov portu, ktorı otvárame
				port,
				GENERIC_READ | GENERIC_WRITE,    // Otvárame na èítanie aj na zápis
				0,                               // Zdie¾anie nebude iadne
				NULL,                            // Bezpeènostné info iadne
				OPEN_EXISTING,                   // Otvárame existujúci (port)
				0,                               // iadne atribúty súboru
				NULL);                           // Musí by NULL

	if ( hCom == INVALID_HANDLE_VALUE ){

		printf("\n Chyba:  Port sa neda otvorit.\n");
		CloseHandle(hCom);
		exit(-1);
	}
	else
		printf("\n OK, port je uspesne otvoreny.\n");

	return hCom;
}

void get_port_settings(HANDLE hCom, DCB *PortDCB){

	GetCommState(hCom,PortDCB);       // Naèítaj aktuálne nastavenia

	printf("Baud rate: %d\n", (int)PortDCB->BaudRate);
//	printf("\n");
//	printf("\n");
//	printf("\n");
//	printf("\n");
//	printf("\n");
//	printf("\n");
//	printf("\n");
//	printf("\n");
}

void set_port_settings(HANDLE hCom, DCB *PortDCB){

	PortDCB->DCBlength = sizeof(DCB);
	PortDCB->BaudRate = 9600;           // Zmeníme nejakı parameter v DCB
//	PortDCB->Parity = (BYTE)(1 || 0);
//	PortDCB->ByteSize;
//	PortDCB->fRtsControl; 				//RTS
//	PortDCB->StopBits;
//	PortDCB->fOutxCtsFlow; 				//CTS
//	PortDCB->fOutxDsrFlow; 				//DSR
//	PortDCB->fDsrSensitivity; 			//DSRs
//	PortDCB->fDtrControl; 				//DTR circiut
//	PortDCB->fOutX;						//XON,XOF

	if(!SetCommState(hCom,PortDCB)){
		printf("Chyba:  Port sa neda nakonfigurovat.\n");
		CloseHandle(hCom);
		exit(-1);
	}
	else
		printf("OK, port je uspesne nakonfigurovany.\n");
}

void menu(){
	printf("---Menu---\n");
	printf("1 - Vypis menu\n");
	printf("2 - Otvor port\n");
	printf("3 - Nastav port\n");
	printf("4 - Zobraz nastavenia portu\n");
}

void send_msg(HANDLE *hCom, unsigned char *msg){

	DWORD Pocet=0;

	WriteFile(
			hCom,   // Handle portu, ktory ste otvorili CreateFile
			msg,  // Smernik (pointer) na msg, ktore chcem vysielat
			1,      // Pocet bytov, ktore chcem vyslat
			&Pocet, // Smernik (pointer) na pocet vyslanych dat
			NULL);  // Musi byt NULL

	if( Pocet != strlen(msg) ){ // Ak pocet prenesenych dat nesuhlasi
		printf("Chyba: Zapis na port sa nepodaril.\n");
		CloseHandle(hCom);
		exit(-1);
	}
	else
		printf("\n OK.");
}

void get_msg(HANDLE *hCom){

	DWORD Pocet=0;            // Poèet prijatıch dát
	unsigned char Data;      // Sem ulo prijatı znak

	PurgeComm(hCom,PURGE_TXCLEAR | PURGE_RXCLEAR);

	ReadFile(
			hCom,         // Handle portu, z ktorého èítame
			&Data,        // Pointer na buffer, kam ukladáme prijaté dáta
			1,            // Poèet bytov, na ktoré èakáme
//			&d,           // Pointer na skutoènı poèet prijatıch dát
			&Pocet,        // Pointer na skutoènı poèet prijatıch dát
			NULL);        // Musí by NULL

	                         // Funkcia ReadFile tu teraz èaká, a kım nieèo
	                         // nepríde, ale môe aj skonèi bez zachyteného znaku,
	                         // ak medzitım vypršia timeouty.

	if( Pocet != 1 )      // Ak poèet prenesenıch dát nesúhlasí
		printf("\n Chyba: Citanie z portu nedalo nic.\n");
	else
		printf("\n OK, z portu sme uspesne prijali [%c].\n",Data);
}

int main(void) {
	setvbuf(stdout, NULL, _IONBF, 0);
	setvbuf(stderr, NULL, _IONBF, 0);


	HANDLE hCom; 		// Handle na objekt typu súbor
	DCB PortDCB; 		// Štruktúra, v ktorej sú uloené parametre
	char c = 0;
	char *port = 	(char *)malloc(6);
	char *msg  = 	(char *)malloc(1024);

//	PortDCB.DCBlength = sizeof(DCB);  // Inicializuj poloku DCBlength

	if((!port) || (!msg)){
		printf("nepodarilo sa alokovat pamat\n");
		return EXIT_FAILURE;
	}

	menu();
	while((c = getchar()) != 27){

		switch(c){

		case '1':
			menu();
			break;

		// Otvorenie portu
		case '2':
			printf("Zadaj port v tvare COMX: ");
			scanf("%s", port);
			hCom = open_port(port);
			break;

		// Nastavenie portu
		case '3':
			set_port_settings(hCom, &PortDCB);
			break;

		// Vypis nastavenia portu
		case '4':
			get_port_settings(hCom, &PortDCB);
			break;

		default:
			break;
		}
	}

	get_port_settings(&hCom, &PortDCB);

	CloseHandle(hCom);   // zatvorime port
	free(port);
	free(msg);

	return EXIT_SUCCESS;
}
