#include <iostream>
//#include <stdio.h>

using namespace std;

class Node{
public:
	//konstruktor s implicitnymi hodnotami value = -1, left = NULL, right = NULL
	Node(int value = -1, Node* left = NULL, Node* right = NULL){
		set_value(value);
		set_left(left);
		set_right(right);
	}

	//pre nastavenie potomkov vezmite impplicitne hodnoty NULL
	void set_left(Node* left = NULL){
		this->left = left;
	}
	void set_right(Node* right = NULL){
		this->right = right;
	}

	//implicitne nastavte hodnotu -1
	void set_value(int value){
		this->value = value;
	}

	int get_value(){
		return value;
	}
	Node* get_left(){
		return left;
	}
	Node* get_right(){
		return right;
	}

private:
	int value;
	Node *left,*right;
};

class Btree{
public:
	//konstruktor vytvori prazdny strom NULL
	Btree(){
		root = NULL;
	}

	// najst prvok ak ho nenajde vrati NULL
	Node* search(int value){
		Node *temp  = root;

		while(temp != NULL ){
			if(temp->get_value() == value ){
				return temp;
			}
			if(temp->get_value() > value )
				temp = temp->get_left();
			else
				temp = temp->get_right();
		}
	}

	//vlozit prvok vklada sa tak aby neboli 2 rovnake hodnoty v strome
	void insert(int value){
		Node *temp  = root;

		if(root == NULL){
			root = new Node(value);
			return;
		}
		while(temp != NULL ){
			if(temp->get_value() == value ){
				return;
			}
			if(value < temp->get_value())
			{
				if(temp->get_left() == NULL)
					temp->set_left(new Node(value));
				else
					temp = temp->get_left();
			}
			else
			{
				if(temp->get_right() == NULL)
					temp->set_right(new Node(value));
				else
					temp = temp->get_right();
			}
		}

	}

	//tlac podstromu od daneho uzla
	void print_subtree(Node* node){
		if(node == NULL )return;
		print_subtree(node->get_left());
		cout << node->get_value() << " " ;
		print_subtree(node->get_right());
	}

	void print_tree(){
		print_subtree(root);
	}


	//opravte rekurzivnu funkciu ktora zisti hlbku uzla sub_tree
	int hlbka(Node* sub_tree, Node* uzol){
		if(sub_tree == uzol)
			return 0;

		if(sub_tree->get_value() <  uzol->get_value())
			return (1 + hlbka(sub_tree->get_right(),uzol));
		else
			return (1 + hlbka(sub_tree->get_left(),uzol));
	}

	//vypis uzlov v danej hlbke
	void print_hlbka(int hlbka, Node *temp){
		if(temp == NULL)
			return;
		if(hlbka == 1){
			cout << temp->get_value()<< " ";
			return;
		}
		print_hlbka(hlbka - 1, temp->get_left());
		print_hlbka(hlbka - 1, temp->get_right());
	}

private:
	Node *root;
};


int main(){
	Btree strom;

	//vytvorte pomocu metody insert(opravte insert) strom
	//            5
	//          3   6
	//         1 4    8						1b

	strom.insert(5);
	strom.insert(3);
	strom.insert(1);
	strom.insert(4);
	strom.insert(6);
	strom.insert(8);
	strom.print_tree();
	//opravte funkciu hlbka a pomocou nej zistite hlbku uzlov 4 a 6(pouzite metodu search)  2b
	cout << endl << strom.hlbka(strom.search(5), strom.search(4)) << endl;

	//dorobte funkciu print_hlbka a pomocou nej vytlacte 3 vrstvu stromu tj 1,4,8			2b
	strom.print_hlbka(3, strom.search(5));

	return 0;
}
