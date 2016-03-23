#include <iostream>

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

         //Uloha 1
        void insert(int value){
                Node* temp = root;

                if(!root){
                	root = new Node(value);
                	return;
                }

                while(1){

                        if(temp->data > value)
                        {
                                if(temp->left != NULL) {
                                        temp = temp->left;
                                }
                                else{
                                        temp->left = new Node(value);
                                        return;
                                }
                        }
                        else{
                                if(temp->right != NULL) {
                                        temp = temp->right;
                                }
                                else {
                                        temp->right = new Node(value);
                                        return;
                                }
                        }
                }
        }

        void print_subtree1(Node* temp){
                if(temp == NULL) return;

                print_subtree1(temp->left);
                cout << temp->data << " ";
                print_subtree1(temp->right);

        }

        void print_subtree2(Node* temp){

                if(temp->left != NULL)
                        print_subtree2(temp->left);
                cout << temp->data << " ";
                if(temp->right != NULL)
                        print_subtree2(temp->right);

        }

        void print_tree(){
                print_subtree1(root);
        }


        //Uloha 4
        bool search(Node* temp, int value){
        	if(temp == NULL)
        		return false;
        	if(temp->data == value)
        		return true;

        	search(temp->left,value);
        	search(temp->right,value);
        }
        //Uloha 5
        /*
        int Leaf_count_node(Node* temp){

        }

        int Leaf_count_tree(){
           ??
        }*/

};






int main(){


   Btree a;

   //Uloha 2
   a.insert(5);
   a.insert(3);
   a.insert(1);
   a.insert(4);
   a.insert(10);
   a.insert(16);
   a.insert(13);
   a.insert(18);


   a.print_tree();
   cout << endl;

   //Uloha 5
   // cout << Leaf_count_tree();

   return 0;
}
