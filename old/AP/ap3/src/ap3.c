#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <string.h>

HANDLE open_port(char * port){

	HANDLE hCom = CreateFile(
//				"COM3:",                         // N�zov portu, ktor� otv�rame
				port,
				GENERIC_READ | GENERIC_WRITE,    // Otv�rame na ��tanie aj na z�pis
				0,                               // Zdie�anie nebude �iadne
				NULL,                            // Bezpe�nostn� info �iadne
				OPEN_EXISTING,                   // Otv�rame existuj�ci (port)
				0,                               // �iadne atrib�ty s�boru
				NULL);                           // Mus� by� NULL

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

	GetCommState(hCom,PortDCB);       // Na��taj aktu�lne nastavenia

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
	PortDCB->BaudRate = 9600;           // Zmen�me nejak� parameter v DCB
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

	DWORD Pocet=0;            // Po�et prijat�ch d�t
	unsigned char Data;      // Sem ulo� prijat� znak

	PurgeComm(hCom,PURGE_TXCLEAR | PURGE_RXCLEAR);

	ReadFile(
			hCom,         // Handle portu, z ktor�ho ��tame
			&Data,        // Pointer na buffer, kam uklad�me prijat� d�ta
			1,            // Po�et bytov, na ktor� �ak�me
//			&d,           // Pointer na skuto�n� po�et prijat�ch d�t
			&Pocet,        // Pointer na skuto�n� po�et prijat�ch d�t
			NULL);        // Mus� by� NULL

	                         // Funkcia ReadFile tu teraz �ak�, a� k�m nie�o
	                         // nepr�de, ale m��e aj skon�i� bez zachyten�ho znaku,
	                         // ak medzit�m vypr�ia timeouty.

	if( Pocet != 1 )      // Ak po�et prenesen�ch d�t nes�hlas�
		printf("\n Chyba: Citanie z portu nedalo nic.\n");
	else
		printf("\n OK, z portu sme uspesne prijali [%c].\n",Data);
}

int main(void) {
	setvbuf(stdout, NULL, _IONBF, 0);
	setvbuf(stderr, NULL, _IONBF, 0);


	HANDLE hCom; 		// Handle na objekt typu s�bor
	DCB PortDCB; 		// �trukt�ra, v ktorej s� ulo�en� parametre
	char c = 0;
	char *port = 	(char *)malloc(6);
	char *msg  = 	(char *)malloc(1024);

//	PortDCB.DCBlength = sizeof(DCB);  // Inicializuj polo�ku DCBlength

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
