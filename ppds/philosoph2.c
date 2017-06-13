#include <stdio.h>
#include <unistd.h>
#include <semaphore.h>
#include <pthread.h>

#include "sync.h"

sem_t sema;
sem_t _fork[5];

void *p(void *v) {
	int i = *(int *)v;
	for (;;) {
		printf("%d: thinking\n", i);

		lock(&sema);
		lock(&_fork[i]);
		lock(&_fork[(i+1)%5]);

		printf("%d: eating\n", i);

		unlock(&_fork[i]);
		unlock(&_fork[(i+1)%5]);
		unlock(&sema);
	}
}

int main() {
	int i;
	pthread_t th[5];
	int id[5];

	inits(&sema, 4);
	for (i = 0; i < 5; i++) {
		inits(&_fork[i], 1);
	}
	
	for (i = 0; i < 5; i++) {
		id[i] = i;
		assert(pthread_create(&th[i], 0, p, &id[i]) == 0);
	}
	pthread_exit(0);
}

