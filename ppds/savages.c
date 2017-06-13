#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

typedef struct {
  sem_t mutex, empty, full;
  int servings;
} pot;

void pot_init(pot *p) {
  sem_init(&p->mutex, 0, 1);
  sem_init(&p->empty, 0, 0);
  sem_init(&p->full, 0, 0);
  p->servings = 0;
}

void cook(pot *p) {
  for (;;) {
    sem_wait(&p->empty);
    p->servings = 10;
    printf("cooked %d servings\n", p->servings);
    sem_post(&p->full);
  }
}

void *savage(void *v) {
  pot *p = v;
  for (;;) {
    sem_wait(&p->mutex);
    if (p->servings == 0) {
      sem_post(&p->empty);
      sem_wait(&p->full);
    }
    p->servings--;
    sem_post(&p->mutex);
    
    printf("eating\n");
    sleep(1);
  }
  return NULL;
}

int main() {

  int i, n = 7;
  pot p;
  pot_init(&p);
  pthread_t th[n];

  for (i=0; i<n; i++) {
    pthread_create(&th[i], NULL, savage, &p);
  }

  cook(&p);
  return 0;
}
