#include<iostream>

//trieda String predstavuje retazec = pole charov ktore si pamata aj velkost

class String{
public:

	//vytvor retazec dlzky size resp. dlzky 3 ak sa pouzije defaultny konstruktor
	//prvky retazca budu '*' - skuste pouzit len metody
	String(int size = 3){
		this->size = size;
		this->string_pointer = new int[size];
		int i;
		for(i = 0; i < size; i++)
			set(i);
	}

	//nastav pismeno na pozicii index na hodnotu letter, ak sa pismeno nezada nastavi sa pismeno na '*'
	void set(int index, char letter = '*'){
		this->string_pointer[index] = letter;
	}
	//vynuluj vsetky prvky retazca (nastav ich na pismeno '*')- mozete pouzit metodu set_nth
	void erase(){
		int i;
		for(i = 0; i < size; i++){
			set(i);
		}
	}
	//vrat pismeno na pozicii index
	int get_letter(int index){
		return string_pointer[index];
	}
	//pridaj pismeno na koniec
	void add_letter(int letter){
		this->size++;
		int * n_strp = new int[size];
		int i;
		for(i = 0; i < size - 1; i++)
			n_strp[i] = string_pointer[i];
		n_strp[size - 1] = letter;
		delete string_pointer;
		this->string_pointer = n_strp;
	}
	//tlac stringu od n-teho pismena, ak sa n nezada ma sa tlacit cely string
	void print(int n = 0){
		for( ; n < size; n++)
			std::cout << (char)this->string_pointer[n];
		std::cout << std::endl;
	}

private:
	int* string_pointer;
	int size;
};

//int main(){
//
//	//vytvor  retazec1 (typu String)  dlzky 3 defaultny konstruktor				= 1b
//	String *retazec1 = new String();
//
//	//vytvor  retazec2 dlzky 10													= 0.5b
//	String *retazec2 = new String(10);
//	//vytlac  prvky oboch retazcov	metodou print								= 0.5b
//
//	retazec1->print();
//	retazec2->print();
//
//	//nastav retazec1 na "abc"													= 1b
//	int i;
//	for(i = 0; i < 3; i++)
//		retazec1->set(i, 97 + i);
////	retazec1->print();
//
//	//vytlac len prvy a druhy znak retazca pole1								= 1b
//	retazec1->print(1);
////	for(i = 0; i < 2; i++)
////		std::cout << (char)retazec1->get_letter(i);
////	std::cout << std::endl;
//
//	//pridaj k retazcu1 pismeno d a vylac										= 1b
//	retazec1->add_letter('d');
//	retazec1->print();
//
//	return 0;
//}
