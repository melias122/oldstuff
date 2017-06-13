#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

#include "sync.h"

sem_t muInsert, noSearch, noInsert;
lightswitch_t lsSearch, lsInsert;

void *deleter(void *v) {
  for (;;) {
    lock(&noSearch);
    lock(&noInsert);

    printf("deleter: deleting ..\n");
    usleep(50);
    
    unlock(&noInsert);
    unlock(&noSearch);
  }
}

void *searcher(void *v) {
  for (;;) {
    ls_lock(&lsSearch, &noSearch);

    printf("searcher: searching for items\n");
    usleep(30);
    
    ls_unlock(&lsSearch, &noSearch);
  }
}

void *inserter(void *v) {
  for (;;) {
    ls_lock(&lsInsert, &noInsert);
    lock(&muInsert);

    printf("inserter: inserting item\n");
    usleep(50);

    unlock(&muInsert);
    ls_unlock(&lsInsert, &noInsert);
  }
}

int main() {
  assert(sem_init(&muInsert, 0, 1) == 0);
  assert(sem_init(&noSearch, 0, 1) == 0);
  assert(sem_init(&noInsert, 0, 1) == 0);
  ls_init(&lsSearch);
  ls_init(&lsInsert);

  pthread_t tDeleter[2];
  pthread_t tSearcher[5];
  pthread_t tInserter[5];

  int i;
  for (i=0; i<2; i++) {
    assert(pthread_create(&tDeleter[i], 0, deleter, 0) == 0);
  }
  
  for (i=0; i<5; i++) {
    assert(pthread_create(&tSearcher[i], 0, searcher, 0) == 0);
    assert(pthread_create(&tInserter[i], 0, inserter, 0) == 0);
  }

  pthread_exit(0);
  return 0;
}
