#ifndef fd_h
#define fd_h

#include "extent_protocol.h"

class yfs_fd {
  std::string payload;
  extent_protocol::attr attr;

 public:
  yfs_fd(std::string payload, extent_protocol::attr attr) {
    this->attr = attr;
    this->payload = payload;
  }

  ~yfs_fd(){
  };

  std::string get_payload() {
    return payload;
  }

  extent_protocol::attr get_attr() {
    return this->attr;
  }

  void set_payload(std::string payload) {
    this->payload = payload;
  }

  void set_attr(extent_protocol::attr attr) {
    this->attr = attr;
  }

};

#endif
