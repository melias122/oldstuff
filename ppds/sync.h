#include <semaphore.h>
#include <assert.h>

#define inits(sem, val) assert(sem_init(sem, 0, val) == 0);
#define lock(sem) assert(sem_wait((sem)) == 0)
#define unlock(sem) assert(sem_post((sem)) == 0)

typedef struct {
  sem_t mutex;
  int counter;
} lightswitch_t;

void ls_init(lightswitch_t *ls) {
  inits(&ls->mutex, 1);
  ls->counter = 0;
}

void ls_lock(lightswitch_t *ls, sem_t *sem) {
  lock(&ls->mutex);
  ls->counter++;
  if (ls->counter == 1) {
    lock(sem);
  }
  unlock(&ls->mutex);
}

void ls_unlock(lightswitch_t *ls, sem_t *sem) {
  lock(&ls->mutex);
  ls->counter--;
  if (ls->counter == 0) {
    unlock(sem);
  }
  unlock(&ls->mutex);
}
