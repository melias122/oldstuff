#include <iostream>

using namespace std;

class Item{

private:
	int data;
	Item* next;

public:
	void set_Item(int data = -1, Item* next = NULL){
		this->data = data;
		this->next = next;
	}

	Item(){
		cout << "Zavolal sa bezparametricky konstruktor" << endl ;
	} // Uloha 1 - problem s duplicitou konstruktora

	Item(int data = -1, Item* next = NULL){
		set_Item(data,next);
		//cout << "Zavolal sa parametricky konstruktor na Item" << endl;
	}

	void print();
	Item* get_next(){
		return next;
	}
	int get_data(){
		return data;
	}
	~Item(){
		cout << "zavolal sa destruktor na Item" << endl;
	}

};

void Item::print(){
        cout << data << " ";
}


class List{
	public:

	List(Item* first = NULL){
		this->first = first;
	}
//	List(List a); //kopirovaci konstruktor
	void push_front(int data); //prototyp metody
	void print();  //prototyp metody
	void push_back(int data);
	void pop_back();
	void pop_front();
	Item* erase(Item* position);            //odpojenie konkretneho prvku z listu, vrati pointer naň (v tejto funkcii nie je prvok vymazany z pamate!!!)
	Item* erase( Item* first, Item* last);   //odpojenie prvkov z listu medzi pointrami first,last(vratane), vrati pointer na first (v tejto funkcii nie su prvky vymazane z pamate!!! - mozete to urobit napr v maine)
	~List(); //destruktor

	private:
    Item* first;
};



Item* List::erase(Item* position){

	Item *temp = first, *prev = NULL;

	while(temp->get_next() != position){
		prev = temp;
		temp = temp->get_next();
	}

	prev->set_Item(prev->get_data(),temp->get_next());
	position->set_Item(position->get_data(), NULL);

	return position;
}

Item* List::erase(Item *first, Item *last){

	return first;
}

void List::push_front(int data){ //definicia metody
	Item* temp = new Item(data,first);   //zavolanie konstruktora
	first = temp;
}

void List::push_back(int data){

	Item *temp = first;

	if(!temp){
		this->first = new Item(data);
		return;
	}

	while(temp->get_next()){
		temp = temp->get_next();
	}

	Item *next = new Item(data);
	temp->set_Item(temp->get_data(), next);
}

void List::pop_back(){

	Item *temp = first, *prev = NULL;


	while(temp->get_next()){
		prev = temp;
		temp = temp->get_next();
	}
	delete(temp);
	prev->set_Item(prev->get_data(), NULL);
}

void List::pop_front(){
	Item *temp = first;
	first = temp->get_next();
	delete temp;
}


void List::print(){

	Item* temp = first;

	while(temp != NULL){
		temp->print();           //pouzivam metodu print pre Item
		//temp = temp->next;       // Uloha 2 nefunguje - next je private -> musim pouzit metodu
		temp = temp->get_next();   //next je private, preto musim pouzit metodu ktora mi vrati pointer na next
	}
	cout << endl;
}



List::~List(){

}

//int main(){
//
//	List a;
//
//	int i;
//
//	for(i = 1; i <= 20; i++)
//		a.push_back(i);
//
//	a.print();
//
//
//
//
//	a.pop_back();
//	a.pop_front();
//	a.print();
//}
