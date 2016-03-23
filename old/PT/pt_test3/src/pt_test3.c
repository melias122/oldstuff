#include <stdio.h>
#include <stdlib.h>
#include <time.h>

struct Item* new_Item(struct Item* next_item , int new_value);
struct Item* add_to_begin(struct Item* begin , int new_value);
void print(struct Item* actual_item);
void add_to_begin2(struct Item** begin_adr);
struct Item* find_previous(struct Item* begin, struct Item* actual_item );
void insert_Item(struct Item* begin , struct Item* actual_item);
void erase_Item(struct Item** begin_adr, struct Item* actual_item);
struct Item * find_value(struct Item* begin, int value);

struct Item* rand_list(int n, int a, int b);
void insertion_sort(struct Item** begin_adr);

struct Item{
  int value;
  struct Item* next;
};

struct Item* new_Item(struct Item* next_item , int new_value){

	struct Item *new_item = (struct Item *) malloc(sizeof(struct Item));

	new_item->value = new_value;
	new_item->next = next_item;

	return new_item;
}
struct Item* add_to_begin(struct Item* begin , int new_value){

	struct Item * new_item = new_Item(begin, new_value);

	return new_item;
}
void print(struct Item* actual_item){

	struct Item *temp = actual_item;

	while(temp != NULL){

		printf("%d ", temp->value);
		temp = temp->next;
	}
	printf("\n");
}
void add_to_begin2(struct Item** begin_adr){

	*begin_adr = new_Item(*begin_adr, 0);
}
struct Item* find_previous(struct Item* begin, struct Item* actual_item ){

	struct Item *temp = begin;

	if(actual_item == begin)
		return NULL;

	while(temp->next != actual_item){
		temp = temp->next;
	}
	return temp;
}
void insert_Item(struct Item* begin , struct Item* actual_item){

	struct Item *new = new_Item(actual_item->next, 7);
	actual_item->next = new;
}
void erase_Item(struct Item** begin_adr, struct Item* actual_item){

	struct Item *temp = NULL;

	if(*begin_adr == actual_item){
		*begin_adr = actual_item->next;
		free(actual_item);
	}
	else{
		temp = *begin_adr;
		while(temp->next != actual_item)
			temp = temp->next;

		temp->next = actual_item->next;
		free(actual_item);
	}
}
struct Item * find_value(struct Item* begin, int value){
	struct Item *temp = begin;

	while(temp != NULL){
		if(temp->value == value)
			break;
		temp = temp->next;
	}
	return temp;
}

struct Item* rand_list(int n, int a, int b){

	struct Item * tmp = NULL;

	int i = 0;
	while(i < n){
		tmp = add_to_begin(tmp, (rand() % b) + a);
		i++;
	}
	return tmp;
}

void insert_in_place(struct Item ** begin_adr, struct Item * actual_item){

	struct Item *tmp = *begin_adr, *tmp_bf = NULL;

	while(1){
		actual_item->next = tmp;
		if(tmp){ // Zaciatok a stred zoznamu
			if(actual_item->value <= tmp->value){
				if(tmp_bf == NULL){
					*begin_adr = actual_item;
					return;
				}
				else {
					tmp_bf->next = actual_item;
					return;
				}
			}
		}
		else if(tmp_bf){	// Koniec zoznamu
			tmp_bf->next = actual_item;
			return;
		}
		else{ // Prazdny zoznam
			*begin_adr = actual_item;
			return;
		}
		tmp_bf = tmp;
		tmp = tmp->next;
	}
}

void insertion_sort(struct Item** begin_adr){
	struct Item *tmp = *begin_adr, *tmp_next = NULL;

	*begin_adr = NULL;
	while(tmp != NULL){
		tmp_next = tmp->next;
		insert_in_place(begin_adr, tmp);
		tmp = tmp_next;
	}
}


// 4 test
void print2(struct Item *begin){
	printf("%d ",begin->value);
	if(begin->next != NULL){
		print2(begin->next);
		return;
	}
	printf("\n");
}

void add_to_back(struct Item* begin, int n, int a, int b){

	if(begin->next != NULL){
		add_to_back(begin->next, n, a, b);
		return;
	}
	else if(n > 0){
		begin->next = new_Item(NULL, (rand() % b) + a);
		add_to_back(begin->next, --n, a, b);
	}
}

void add_to_front(struct Item** begin, int n, int a, int b){

	if(n != 0){
		*begin = new_Item(*begin,rand()%b + a);
		add_to_front(begin, --n, a, b);
	}
}

void swap(int *a, int *b){
	int c = *a;
	*a = *b;
	*b = c;
}

void swap2(struct Item **a, struct Item **b){

	struct Item *temp = *a;
	*a = *b;
	temp->next = (*a)->next;
	(*a)->next = temp;
}

int pivot(int *arr, int left, int right){

	int i;
	int p[3];

	for (i = 0; i < 3; ++i) {
		p[i] = rand()%(right - left) + left;
		printf("%d ", arr[p[i]]);
	}
	printf("\n");

	if(arr[p[0]] < arr[p[1]]){ 		// 1 2 3
		if(arr[p[1]] < arr[p[2]]) 	// 1 2 3
			return p[1]; 			// 2
		else						// 1 3 2
			return p[2]; 			// 2
	}
	else { 							// 2 1 3
		if(arr[p[0]] < arr[p[2]]) 	// 2 < 3
			return p[0]; 			// 2
		else 						// 3 1 2
			return p[2];
	}
}

int partition2(int *arr, int left, int right){

	int p = rand()%(right - left) + left;
	int j = left;
	int k = right - 1;

	swap(&arr[p], &arr[right]);
	while(j <= k){

		if(arr[j] > arr[right]){
			if(arr[k] <= arr[right]){
				swap(&arr[j], &arr[k]);
				j++; k--;
			}
			else
				k--;
		}
		else
			j++;
	}
	swap(&arr[j], &arr[right]);
	return j;
}

// arr[right] je pivot
// arr[i] je aktualny prvok pri prechode
// arr[left] je prvok pre vymenu
int partition1(int *arr, int left, int right){

	int i;
	int p = rand()%(right - left) + left; // nahodny vyber pivota

	swap(&arr[p], &arr[right]);
	for(i = left; i < right; i++){
		if(arr[i] < arr[right]){
			swap(&arr[left], &arr[i]);
			left++;
		}
	}
	swap(&arr[right], &arr[left]);
	return left;
}

void quick_sort(int *arr, int left, int right){

	if(left >= right)
		return;

	int index = partition2(arr, left, right);

	quick_sort(arr, left, index - 1);
	quick_sort(arr, index + 1, right);

}


int main(void) {

	int i;
	struct Item *begin = NULL, *random_list = NULL, *temp = NULL;

	srand(time(NULL));

//	for(i = 6; i > 0; i--){
//		begin = add_to_begin(begin, i);
//	}
//	add_to_begin2(&begin);
//	print(begin);
//	swap2(&begin, &begin->next);
//	print(begin);
//	temp = find_previous(begin, NULL); // end
//	printf("%d\n", temp->value);
//	temp = find_previous(begin, temp); // bef end
//	printf("%d\n", temp->value);
//	temp = NULL;
//	while((temp = find_previous(begin, temp)) != NULL)
//		printf("%d ", temp->value);
//	printf("\n");
//	insert_Item(begin, find_previous(begin, NULL));
//	print(begin);
//	erase_Item(&begin, begin);
//	print(begin);
//	temp = find_value(begin, 3);
//	if(temp)
//		printf("%d\n", temp->value);
//	erase_Item(&begin, find_value(begin,5));
//	print(begin);
//	random_list = rand_list(8, 1, 20);
//	print2(random_list);
//	temp = new_Item(NULL,22);
//	insert_in_place(&random_list,temp);
//	insertion_sort(&random_list);
//	printf("\n");
//	print2(random_list);
//	add_to_back(random_list,4);
//	printf("\n");
//	print2(random_list);

//	add_to_front(&temp, 10, 5, 15);
//	add_to_back(temp,4,1000,10000);
//	print2(temp);

	int size = 100;

	int *arr = (int *) malloc(size * sizeof(int));
	for (i = 0; i < size; ++i) {
		arr[i] = rand()%10;
		printf("%d ", arr[i]);
	}
	printf("\n");
//	int p = pivot(arr, 0, size);
//	printf("%d", arr[p]);
	quick_sort(arr,0, size - 1);
	for (i = 0; i < size; ++i) {
		printf("%d ", arr[i]);
	}

	return EXIT_SUCCESS;
}
