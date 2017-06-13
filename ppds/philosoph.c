#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdlib.h>

#define panic(code, msg) fprintf(stderr, "%d, %s" , __LINE__, msg); exit(code)
#define check(func) if (func != 0) { panic(1, "error"); }
#define N 5

typedef struct {
  sem_t mu;
  sem_t sema[N];
} forks;

typedef struct {
  forks *forks;
  int i;
} args;

void forks_init(forks *f, int n) {
  int i;
  check(sem_init(&f->mu, 0, n-1));
  for (i = 0; i < n; i++) {
    check(sem_init(&f->sema[i], 0, 1));
  }
}

void think() {
  printf("thinking\n");
  sleep(1);
}

void get_forks(forks *f, int i) {
  check(sem_wait(&f->mu));
  check(sem_wait(&f->sema[i]));
  check(sem_wait(&f->sema[(i+1)%N]));
}

void eat() {
   printf("eating\n");
   sleep(1);
}

void put_forks(forks *f, int i) {
  check(sem_post(&f->sema[i]));
  check(sem_post(&f->sema[(i+1)%N]));
  check(sem_post(&f->mu));
}

void *philosoph(void *arg) {
  if (arg == NULL) {
    panic(1, "arg == NULL");
  }
  
  args *args = arg;
  for (;;) {
    think();
    get_forks(args->forks, args->i);
    eat();
    put_forks(args->forks, args->i);
  }
}

int main() {
  int i;
  pthread_t thread[N];
  forks forks;
  
  forks_init(&forks, N);
  for (i=0;i<N;i++) {
    printf("creating %d\n", i );
    args *a = malloc(sizeof(*a));
    a->i = i;
    a->forks = &forks;
    pthread_create(&thread[i], NULL, philosoph, a);
  }
  for (i=0;i<N;i++) {
    pthread_join(&thread[i], NULL);
  }
  return 0;
}
