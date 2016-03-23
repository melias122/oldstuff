#include <iostream>
#include <stdio.h>
#include <vector>
#include <list>

using namespace std;


class Hash_table{

public:

	//konstruktor ktory vytvori tabulku danej velkosti a naplni ju przadnymi stringami
	Hash_table(unsigned size){

		this->size = size;
		this->tab = vector< list<string> >(size);

		for(unsigned i = 0; i < tab.size(); i++){
			this->tab[i].push_back("");
		}
	}

	//vrat hash retazca
	unsigned hash_function(string str){
		unsigned i;
		unsigned hash = 534;

		for(i = 0; i < str.length(); i++){
			hash = (hash << 7) + str[i];
		}
		return hash%tab.size();
	}

	void insert(string str){
		tab[hash_function(str)].push_front(str);
	}

	//najdi retazec a vrat nan iterator
	list<string>::iterator search(string str){

		unsigned hash = this->hash_function(str);
		list<string>::iterator it = tab[hash].begin();

		while(it != (tab[hash]).end()){
			if((*it == str) || (*it == ""))
				return it;
//			if(*it == "")
//				return it;
			it++;
		}
		return it;
	}

	//vymaz retazec
	void del(string str){
		if(str == "")
			return;
		else
			tab[this->hash_function(str)].remove(str);
	}

	//vytlac obsah tabulky - stringy s rovnakym hashom sa vytlacia do riadku
	void print(){
//		cout << "Velkost tabulky: " << tab.size() << endl;
		for(unsigned i = 0; i < tab.size(); i++){
//			cout << i << ": ";
			for(list<string>::iterator it = tab[i].begin(); it != tab[i].end(); it++){
				cout << *it << " ";
			}
			cout << endl;
		}
	}

private:
		unsigned size;
		vector< list<string> > tab;
};

int main() {

	Hash_table h(5);


	  h.print();
	  //vytlacenie spravnej tabulky 1.5b

	  h.insert("fero");
	  h.insert("jano");
	  h.insert("jozo");
	  h.insert("karol");
	  //vytlacenie spravnej tabulky 1b

	  h.del("feri");
	  h.del("jano");
	  //vytlacenie ("feri" nesmie spadnut)spravnej tabulky 1b

	  cout << *(h.search("fero")) << endl;
	  cout << *(h.search("jozo")) << endl;
	  //spravne vytlacenie jozo a a fero  1.5b

	return 0;
}
