// the extent server implementation

#include "extent_server.h"
#include <sstream>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

extent_server::extent_server() {
  pthread_mutex_init(&mtx, NULL);
  int res;
  put(0x00000001, "", res);
}


int extent_server::put(extent_protocol::extentid_t id, std::string buf, int &)
{
  // You fill this in for Lab 2.
  ScopedLock lock(&mtx);

  extent_protocol::attr attr;
  attr.atime = time(NULL);
  attr.ctime = attr.atime;
  attr.mtime = attr.atime;
  attr.size = buf.size();

  yfs_fd* fd = new yfs_fd(buf, attr);

  fd_map[id] = fd;

  return extent_protocol::OK;
}

int extent_server::get(extent_protocol::extentid_t id, std::string &buf)
{
  // You fill this in for Lab 2.
  ScopedLock lock(&mtx);

  if (fd_map.find(id) != fd_map.end()) {
    extent_protocol::attr attr = fd_map[id]->get_attr();
    attr.atime = time(NULL);
    buf = fd_map[id]->get_payload();

    return extent_protocol::OK;
  }

  return extent_protocol::IOERR;
}

int extent_server::getattr(extent_protocol::extentid_t id, extent_protocol::attr &a)
{
  // You fill this in for Lab 2.
  // You replace this with a real implementation. We send a phony response
  // for now because it's difficult to get FUSE to do anything (including
  // unmount) if getattr fails.
  ScopedLock lock(&mtx);

  if (fd_map.find(id) != fd_map.end()) {
    a = fd_map[id]->get_attr();

    return extent_protocol::OK;
  }

  return extent_protocol::IOERR;
}

int extent_server::remove(extent_protocol::extentid_t id, int &)
{
  // You fill this in for Lab 2.
  ScopedLock lock(&mtx);

  std::map<extent_protocol::extentid_t, yfs_fd*>::iterator iter;
  iter = fd_map.find(id);
  if (iter != fd_map.end()) {
    fd_map.erase(iter);

    return extent_protocol::OK;
  } else {
    return extent_protocol::NOENT;
  }

  return extent_protocol::IOERR;
}

