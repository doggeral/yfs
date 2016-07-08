// the lock server implementation

#include "lock_server.h"
#include <sstream>
#include <stdio.h>
#include <unistd.h>
#include "pthread.h"
#include <arpa/inet.h>

lock_server::lock_server():
  nacquire (0)
{
  pthread_mutex_init(&mtx, NULL);
}

lock_protocol::status
lock_server::stat(int clt, lock_protocol::lockid_t lid, int &r)
{
  lock_protocol::status ret = lock_protocol::OK;
  printf("stat request from clt %d\n", clt);
  r = nacquire;
  return ret;
}

lock_protocol::status
lock_server::acquire(int clt, lock_protocol::lockid_t lid, int &r)
{
  lock_protocol::status ret = lock_protocol::OK;

  pthread_mutex_lock(&mtx);

  std::map<lock_protocol::lockid_t, locks*>::iterator iter = lock_map.find(lid);

  // find the lock
  if(iter != lock_map.end()) {
	  while(iter->second->get_state() != lock_state::FREE) {
		  pthread_cond_wait(&(iter->second->get_cond()), &mtx);
	  }

	  iter->second->set_state(lock_state::LOCKED);
  } else {
	  lock_map.insert(pair<lock_protocol::lockid_t, locks*>(lid, new locks(lock_state::LOCKED)));
  }

  pthread_mutex_unlock(&mtx);

  return ret;
}

lock_protocol::status
lock_server::release(int clt, lock_protocol::lockid_t lid, int &r)
{
  lock_protocol::status ret = lock_protocol::OK;

  pthread_mutex_lock(&mtx);

  std::map<lock_protocol::lockid_t, locks*>::iterator iter = lock_map.find(lid);

  // find the lock
  if (iter != lock_map.end()) {
    iter->second->set_state(lock_state::FREE);
    pthread_cond_signal(&(iter->second->get_cond()));
  } else {
    ret = lock_protocol::IOERR;
  }

  pthread_mutex_unlock(&mtx);

  return ret;
}

