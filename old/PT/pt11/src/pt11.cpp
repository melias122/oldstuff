#include <iostream>
#include <vector>
#include <list>


using namespace std;

class  Node{

public:

    int data;
    Node *left,*right;

        Node(int _data = -1, Node *_left = NULL, Node* _right = NULL){
                data = _data;
                left = _left;
                right = _right;
        }


};

class Btree{

public:

        Node * root;

        Btree(Node* _root = NULL){
                root = _root;
        }

        void insert(int value){

        	Node* temp =root;

        	if(temp == NULL){
        		root = new Node(value);
        		return;
        	}

        	while(1){

        		if(value < temp->data){

        			if(temp->left != NULL) {
        				temp = temp->left;
//        				continue;
        			}
        			else{
        				temp->left = new Node(value);
        				break;
        			}
        		}
        		else{

        			if(temp->right != NULL) {
        				temp = temp->right;
//        				continue;
        			}
        			else {
        				temp->right = new Node(value);
        				break;
        			}
        		}
        	}
        }

        void print_level_order(){

        	list<Node *> q;
        	cout << "vypis stromu: "  ;

        	q.push_back(root);

        	while(q.size()){

        		if(q.front()->left){
        			q.push_back(q.front()->left);
        		}
        		if(q.front()->right){
        			q.push_back(q.front()->right);
        		}
        		cout << q.front()->data << " ";
        		q.pop_front();
        	}
        	cout << endl;
        }

        void erase(int value){
        	remove(value);
        }

        Node* search(int value){

        	Node *temp  = root;

        	while(temp != NULL ){

        		if(temp->data == value ){
        			return temp;
        		}

        		if(value < temp->data)
        			temp = temp->left;
        		else
        			temp = temp->right;
        	}
        	return NULL;
        }

//bool BST<T>::remove(const T & itemToDelete)
bool remove(const int value){
	return remove(root, value);
}

//bool BST<T>::remove(Node<T>* & ptr, const T& key)
bool remove(Node* & ptr,const int key)       //helper remove function
{
        if (ptr==NULL)
        	return false;   // item not in BST

        if (key < ptr->data)
                remove(ptr->left, key);
        else if (key > ptr->data)
                remove(ptr->right, key);
        else
        {
                Node *temp;

                if (ptr->left==NULL)
                {
                        temp = ptr->right;
                        delete ptr;
                        ptr = temp;
                }
                else if (ptr->right==NULL)
                {
                        temp = ptr->left;
                        delete ptr;
                        ptr = temp;
                }
                else    //2 children
                {
                        temp = ptr->right;
                        Node *parent = NULL;

                        while(temp->left!=NULL)
                        {
                                parent = temp;
                                temp = temp->left;
                        }
                        ptr->data = temp->data;
                        if (parent != NULL)
                                remove(parent->left, parent->left->data);
                        else
                                remove(ptr->right, ptr->right->data);
                }
        }
        return false;
}

};

class Hash_table{
public:
        Hash_table(int size = 6){
        	table = vector<Btree>(size);
        }
        int hash_function(int value){

        	if(value < 0){
        		value = value*(-1);
        	}

        	int hash = 534;

        	hash = (hash << 3) + value;

        	return hash%table.size();
        }
        void insert(int value){
        	(table[hash_function(value)]).insert(value);
        }
        void erase(int value){
        	table[hash_function(value)].erase(value);
        }
        void print(){
        	for(unsigned i = 0; i < table.size(); i++){
        		table[i].print_level_order();
        	}
        }
        vector<Btree> table;
};





int main(){

        Btree strom;

        //vyrobte strom ktoreho vypis pomocou level orderu bude 5  3 8   1 4 7 9  0 2 6 10
        strom.insert(5);
        strom.insert(3);
        strom.insert(8);
        strom.insert(1);
        strom.insert(4);
        strom.insert(7);
        strom.insert(9);
        strom.insert(0);
        strom.insert(2);
        strom.insert(6);
        strom.insert(10);
        //korekne nacitany strom                                2b

        strom.print_level_order();
        //vypis  5  3 8   1 4 7 9  0 2 6 10             =2b

        //vymaz prvky 1 0 11 a vypis
        strom.erase(1);
        strom.erase(0);
        strom.erase(11);
        cout << "Po zmazani ";
        strom.print_level_order();
        //korektne vymazanie prvkov                             2b

        //vytvor tabulky s 5timi a 6timi prvkami
        Hash_table h1(5),h2(6);
        //korektne nastavenie velkosti tabuliek 2b
        cout << endl << "H1 size: " << h1.table.size() << endl;
        cout << "H2 size: " << h2.table.size() << endl;

        //vytlac tabulku h1 s 5timi prvkami a h2 s defaulnymi 6timi prvkami
        //do ktorej sa vlozia nasledujuce prvky
        h2.insert(0);
        h2.insert(1);
        h2.insert(2);
        h2.insert(3);
        h2.insert(4);
        h2.insert(5);
        h2.insert(6);
        cout << endl << "---H2---" << endl;
        h2.print();

        h1.insert(3);
        h1.insert(8);
        h1.insert(11);
        h1.insert(7);
        h1.insert(128);
        h1.insert(8);
        h1.insert(67);
        h1.insert(14);
        h1.insert(39);
        h1.insert(-125);
        cout << endl << "---H1---" << endl;
        h1.print();
        //korektne vytlacene tabulky                     2b

        //vymaz prvky 14 7 128 z tabulky h1 a vytlac ju
        h1.erase(14);
        h1.erase(7);
        h1.erase(128);
        cout << endl << "---H1-erased---" << endl;
        h1.print();
        //korektne vtlacena tabulka h1              2b

        return 0;
}
