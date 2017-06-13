#include <stdio.h>
#include <unistd.h>
#include <semaphore.h>
#include <pthread.h>
#include <stdlib.h>

sem_t mut;
int   num_res[3] = {0, 0, 0};

sem_t age;
sem_t res[3]; // 0 == tobacco, 1 == paper, 2 == match
sem_t smo[3];

void init() {
  int i;
  sem_init(&mut, 0, 1);
  sem_init(&age, 0, 1);
  for (i = 0; i < 3; i++) {
    sem_init(&res[i], 0, 0);
    sem_init(&smo[i], 0, 0);
  }
}

void *agent(void *v) {
  int i, j;
  for (;;) {

    do {
      i = rand()%3;
      j = rand()%3;
    } while (i == j);

    // printf("agent: waiting\n");
    sem_wait(&age);
    // printf("agent: pushing %d, %d\n", i, j);
    sem_post(&res[i]);
    sem_post(&res[j]);
  }
  return NULL;
}

void *pusher(void *v) {
  int x = *(int *)v;
  int i, j, k;

  i = (x + 0)%3;
  j = (x + 1)%3;
  k = (x + 2)%3;

  printf("pusher: %d, %d, %d\n", i, j, k);
  for (;;) {
    sem_wait(&res[i]);
    
    sem_wait(&mut);
    if (num_res[j]) {
      num_res[j]--;
      sem_post(&smo[k]);
    } else if (num_res[k]) {
      num_res[k]--;
      sem_post(&smo[j]);
    } else {
      num_res[i]++;
    }
    sem_post(&mut);
  }
  return NULL;
}

void *smoker(void *v) {

  static char *name[] = {
    "tobacco",
    "paper",
    "match",
  };

  int i = *(int*)v;
  for (;;) {
    sem_wait(&smo[i]);

    //printf("%s smoker: making cigarette\n", name[i]);
    sem_post(&age);
    
    printf("%s smoker: smoking\n", name[i]);
    sleep(1);
  }
  return NULL;
}

int main() {
  int i;
  pthread_t t, t1[6];
  int id[] = {0, 1, 2};
  
  init();
  pthread_create(&t, 0, agent, 0);
  for (i=0; i<3; i++) {
    pthread_create(&t1[i], 0, pusher, &id[i]);
    pthread_create(&t1[i+3], 0, smoker, &id[i]);
  }
  pthread_exit(0);
  return 0;
}
