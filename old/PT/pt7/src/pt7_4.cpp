#include<iostream>

using namespace std;

//trieda String predstavuje retazec = pole charov ktore si pamata aj velkost

class String{
public:
	//vytvor retazec dlzky size resp. dlzky 3 ak sa pouzije defaultny konstruktor
	//prvky retazca budu '*' - skuste pouzit len metody
	String(int size = 3)
	{   //konstruktor
		this->size = size;
		this->string_pointer = new int[size];
		for(int i = 0; i < size; i++)
		{
			set(i);
		}
	}

	String(String &tocopy) {  //pouzitie kopirovacie konstruktora
			size=tocopy.size;
			string_pointer=new int[size];
			for(int i=0; i<size; i++) {
				string_pointer[i]=tocopy.string_pointer[i];
			}
		}
	~String()
	{



	}

	//nastav pismeno na pozicii index na hodnotu letter, ak sa pismeno nezada nastavi sa pismeno na '*'
	void set(int index, char letter = '*')
	{
		string_pointer[index] = letter;
	}
	//vynuluj vsetky prvky retazca (nastav ich na pismeno '*')- mozete pouzit metodu set_nth
	void erase()
	{
		for(int i = 0; i < size; i++)
		{
			set(i);
		}
	}
	//vrat pismeno na pozicii index
	int get_letter(int index)
	{
		return this->string_pointer[index];
	}

	//pridaj pismeno na koniec
	void add_letter(int letter){
		int i;
		int *temp = new int(size+1);
		for(i = 0; i < size; i++)
			temp[i] = string_pointer[i];
		temp[i] = letter;
		string_pointer = temp;
		size++;
	}
	//tlac stringu od n-teho pismena, ak sa n nezada ma sa tlacit cely string
	void print(int n = 0)
	{
		for(int i = n; i < size; i++)
		cout << (char)string_pointer[i];
	}

private: int* string_pointer;
		 int size;
};

void main(){

	//vytvor  retazec1 (typu String)  dlzky 3 defaultny konstruktor				= 1b
	String retazec1;

	//vytvor  retazec2 dlzky 10													= 0.5b
	String retazec2(10);

	//vytlac  prvky oboch retazcov	metodou print								= 0.5b
	retazec1.print();
	cout << endl;
	retazec2.print();
	cout << endl;

	//nastav retazec1 na "abc"													= 1b
	for(int i = 0; i < 3; i++)
		retazec1.set(i,'a'+i);
	retazec1.print();
	cout << endl;
	//vytlac len prvy a druhy znak retazca pole1								= 1b
	for(int i = 0; i < 2; i++)
		cout << (char)retazec1.get_letter(i);
	cout << endl;
	//pridaj k retazcu1 pismeno d a vylac										= 1b
	retazec1.add_letter('d');
	retazec1.print();
	cout << endl;
	String retazec3(retazec1);  //priklad na kop. konstruktor skopiruje retazec1 do retazcu 3
	retazec3.print();
	//retazec3.~String();  //vypise skopirovany
	delete retazec3;
	cout<<endl;
	retazec3.print();  //vypis po odstraneni
	/*retazec1.erase();
	retazec1.print();*/
	system("pause");

}

