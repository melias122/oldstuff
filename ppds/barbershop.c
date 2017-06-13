#include <stdio.h>
#include <unistd.h>
#include <semaphore.h>
#include <pthread.h>

typedef struct {
  int n, customers;
  sem_t mutex;
  sem_t customer, barber;
  sem_t customerDone, barberDone;
  sem_t balk;
} shop_t;

void shop_init(shop_t *s, int n) {
  s->n = n;
  s->customers = 0;
  sem_init(&s->mutex, 0, 1);
  sem_init(&s->customer, 0, 0);
  sem_init(&s->customerDone, 0, 0);
  sem_init(&s->barber, 0, 0);
  sem_init(&s->barberDone, 0, 0);
  sem_init(&s->balk, 0, 0);
}

void *customer(void *v) {
  shop_t *s = v;
  for (;;) {
    sem_wait(&s->mutex);
    if (s->customers == s->n) {
      sem_post(&s->mutex);

      //printf("balk\n");
      // wait
      sem_wait(&s->balk);
      continue;
    }
    s->customers++;
    sem_post(&s->mutex);

    sem_post(&s->customer);
    sem_wait(&s->barber);

    // get haircut

    sem_post(&s->customerDone);
    sem_wait(&s->barberDone);

    sem_wait(&s->mutex);
    s->customers--;
    sem_post(&s->mutex);
    sem_post(&s->balk);
  }
  return NULL;
}

void *barber(void *v) {
  shop_t *s = v;
  for (;;) {
    sem_wait(&s->customer);
    sem_post(&s->barber);

    // cutting hair
    printf("cutting hair\n");
    sleep(1);

    sem_wait(&s->customerDone);
    sem_post(&s->barberDone);
  }
  return NULL;
}

int main() {
  pthread_t th[10];
  shop_t shop;
  int i;
  
  shop_init(&shop, 4);
  pthread_create(&th[0], 0, barber, &shop);

  for (i=1; i < 10; i++) {
    pthread_create(&th[i], 0, customer, &shop);
  }

  pthread_exit(NULL);
  
  return 0;
}
