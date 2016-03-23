#include <iostream>
#include <fstream>
#include <stdlib.h>

using namespace std;

void menu(){

	cout << "Menu: vyber si operáciu" << endl;
	cout << "1 - Nacitaj slovník zo slovnik.txt" << endl;
	cout << "2 - Nacitaj text zo subora text.txt 1b" << endl;
	//ak EN slovo v slovniku nieje ponecha ho tj. vypise neprelozene
	cout << "3 - Preloz text pomocou slovnika a vytlac prelozeny text + uloz EN slova,ktore niesu v slovniku do extra subora neprelozitelne.txt  1b" << endl;
	//postupne ponuka slovicka uzivatelovi, ktory manualne zadava ich preklady do slovenciny.
	cout << "4 - dopln preklady slovicok z neprelozitelne.txt do slovnik.txt a uloz doplneny slovnik. 1b" << endl;
	cout << "5 - vytlac slovnik podla abecedy 1b" << endl;
	//aplikacia umozni uzivatelovi si vybrat pismeno
	cout << "6 - vytlac slova zo slovnika zacinajuce na dane pismeno 1b" << endl;
}

void v1(){

	fstream f;

}

int main() {

	unsigned char volba;

	while(true){

		menu();
		cin >> volba;

		switch (volba) {
			case '1': break;
			case '2': break;
			case '3': break;
			case '4': break;
			case '5': break;
			case '6': break;

			case '0':
				cout << "Bye bye.." << endl;
				exit(0);
				break;

			default: break;
		}
	}
	return 0;
}
