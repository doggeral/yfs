#ifndef lock_h
#define lock_h

#include <pthread.h>

enum lock_state {
  FREE,
  LOCKED
};

class locks {

 protected:
  int lock_state;
  pthread_cond_t cond;


 public:
  locks();
  locks(int lock_state);
  ~locks() {};
  int get_state();
  pthread_cond_t& get_cond();
  void set_state(int lock_state);

};

#endif
