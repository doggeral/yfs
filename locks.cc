#include "locks.h"

locks::locks() {
  this->lock_state = lock_state::FREE;
  pthread_cond_init(&(this->cond), NULL);
}

locks::locks(int lock_state) {
  this->lock_state = lock_state;
  pthread_cond_init(&(this->cond), NULL);
}

int
locks::get_state() {
  return this->lock_state;
}

pthread_cond_t&
locks::get_cond(void) {
  return this->cond;
}

void
locks::set_state(int lock_state) {
  this->lock_state = lock_state;
}
