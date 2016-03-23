#include <stdio.h>

struct Node // vrchol stromu
{
	struct Node *parent; // rodic vrcholu; pre koren stromu to je NULL
	struct Node *left, *right; // ak vetva nepokracuje je to NULL
	char *name; // meno vo vrchole
}Node;

struct Tree // strom
{
	struct Node *root; // koren stromu
}Tree;

struct Node *predchodca(struct Node *node);


// vrati vrchol stromu, ktoreho hodnota je najblizsia mensia ako v 'node'
struct Node *predchodca(struct Node *node)
{
	if (node && node->parent) {
        return node->parent;
    }
    else {
        return NULL;
    }
}

int main(void){

    return 0;
}