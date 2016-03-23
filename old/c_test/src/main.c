#include <stdio.h>
#include <stdlib.h>

typedef struct Uzol{
	int value;
	struct Uzol *next;
}UZOL;

void print(UZOL* actual_item){

	UZOL *temp = actual_item;

	while(temp != NULL){

		printf("%d ", temp->value);
		temp = temp->next;
	}
	printf("\n");
}

void pridaj(UZOL **tmp, int i, int n){

	int j;
	UZOL *u = NULL;

	for(j = 1; j <= i; j++){
		u = (UZOL *) malloc(sizeof(UZOL));
		u->value = i;
		u->next = *tmp;
		*tmp = u;
	}

	if(i < n){
		pridaj(tmp, ++i, n);
	}
	else if (i > 0){
		pridaj(tmp, --i, --n);
	}
}

void swap(UZOL **a, UZOL **b){

	UZOL *temp = *a;
	*a = *b;
	temp->next = (*a)->next;
	(*a)->next = temp;
}

void swap_nodes(UZOL **start, int a, int b){

	int i;
	UZOL *sw1 = NULL, *sw2 = NULL, *tmp = *start;

	i = 0;
	while(tmp != NULL){
		if(i == a){
			sw1 = tmp;
		}
		if(i == b){
			sw2 = tmp;
		}
		if((sw1 != NULL) && (sw2 != NULL)){
			break;
		}
		i++;
		tmp = tmp->next;
	}
//	swap(&sw1, &sw2);
	i = sw1->value;
	sw1->value = sw2->value;
	sw2->value = i;
}

void eraseMax(UZOL ** begin){

	UZOL *temp = *begin, *prev = NULL, *fr = NULL;
	int max = 0;

	while(temp != NULL){
		if(temp->value > max)
			max = temp->value;
		temp = temp->next;
	}

	temp = *begin;
	while(temp != NULL){
		if(temp->value == max){
			if(temp == *begin){
				*begin = temp->next;
			}
			fr = temp;
			prev->next = temp->next;
		}
		if(fr){
			temp = temp->next;
			free(fr); fr = NULL;
		}
		else{
			prev = temp;
			temp = temp->next;
		}
	}

}

int main(void){

	int i = 0;
	UZOL * tmp = NULL;

	pridaj(&tmp, 1, 5);
	printf("pred: ");
	print(tmp);
	eraseMax(&tmp);

	for(i = 0; i < 5; i++){
		swap_nodes(&tmp,9 - i, 0 + i);
		swap_nodes(&tmp,19 - i, 10 + i);
	}
	printf("po: ");
	print(tmp);

	return 0;
}
