#include <stdio.h>
#include <pthread.h>

#include "sync.h"

sem_t empty, mpMale, mpFemale;
lightswitch_t lsMale, lsFemale;

void *man(void *v) {
  for(;;) {
    ls_lock(&lsMale, &empty);
    lock(&mpMale);

    printf("man: entering\n");

    unlock(&mpMale);
    ls_unlock(&lsMale, &empty);
  }
}

void *woman(void *v) {
  for (;;) {
    ls_lock(&lsFemale, &empty);
    lock(&mpFemale);
    
    printf("woman: entering\n");

    unlock(&mpFemale);
    ls_unlock(&lsFemale, &empty);
  }
}

int main() {
  inits(&empty, 1);
  inits(&mpMale, 3);
  inits(&mpFemale, 3);
  ls_init(&lsMale);
  ls_init(&lsFemale);

  int i;
  pthread_t _man[5];
  pthread_t _woman[6];

  for (i=0;i<5;i++) {
    pthread_create(&_man[i], 0, man, 0);
  }

  for (i=0;i<6;i++) {
    pthread_create(&_woman[i], 0, woman, 0);
  }
  pthread_exit(0);
  
  return 0;
}
