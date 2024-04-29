#ifndef __EPOLLER_H__
#define __EPOLLER_H__

#include <string>
#include <sys/socket.h>
#include <sys/epoll.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <iostream>
#include <unordered_map>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>

#define MAX_PENDING 1024
#define BUFFER_SIZE 1024

class Handler {
public:
  virtual ~Handler() {};
  virtual int handle(epoll_event e) = 0;
};

/*
* epoll event 轮询
*/
class IOLoop {
public:
  static IOLoop *Instance() {
    static IOLoop instance;
    return &instance;
  }

  ~IOLoop() {
    for (auto it : handlers_) {
      delete it.second;
    }
  }

  void start() {
    const uint64_t MAX_EVENTS = 10;
    epoll_event events[MAX_EVENTS];
    while (true) {
      int nfds = epoll_wait(epfd_, events, MAX_EVENTS, -1);
      for (int i = 0; i < nfds; i++) {
        int fd = events[i].data.fd;
        Handler* handler = handlers_[fd];
        handler->handle(events[i]);
      }
    }
  }

  void addHandler(int fd, Handler* handler, unsigned int events) {
    handlers_[fd] = handler;
    epoll_event e;
    e.data.fd = fd;
    e.events = events;
    
    if (epoll_ctl(epfd_, EPOLL_CTL_ADD, fd, &e) < 0) {
      std::cout << "Failed to insert handler to epoll" << std::endl;
      return;
    }
  }

  void modifyHandler(int fd, unsigned int events) {
    epoll_event e;
    e.events = events;
    epoll_ctl(epfd_, EPOLL_CTL_MOD, fd, &e);
  }

  void removeHandler(int fd) {
    Handler* handler = handlers_[fd];
    handlers_.erase(fd);
    delete handler;
    epoll_ctl(epfd_, EPOLL_CTL_DEL, fd, nullptr);
  }

private:
  IOLoop() {
    epfd_ = epoll_create1(0);
    if (epfd_ < 0) {
      std::cout << "Failed to create epoll" << std::endl;
      exit(1);
    }
  }

private:
  int epfd_;
  std::unordered_map<int, Handler*> handlers_;
};

class EchoHandler : public Handler {
public:
  EchoHandler() {}
  virtual int handle(epoll_event e) override {
    int fd = e.data.fd;
    if (e.events & EPOLLHUP) {
      IOLoop::Instance()->removeHandler(fd);
      return -1;
    }

    if (e.events & EPOLLERR) {
      return -1;
    }

    if (e.events & EPOLLOUT) {
      if (received > 0) {
        std::cout << "Writing: " << buffer << std::endl;
        if (send(fd, buffer, received, 0) != received) {
          std::cout << "error writing to socket" << std::endl;
        }
      }

      IOLoop::Instance()->modifyHandler(fd, EPOLLIN);
    }

    if (e.events & EPOLLIN) {
      std::cout << "read" << std::endl;
      received = recv(fd, buffer, BUFFER_SIZE, 0);
      if (received > 0) {
        buffer[received] = 0;
        std::cout << "Reading: " << buffer << std::endl;
        if (strcmp(buffer, "stop") == 0) {
          std::cout << "stop-----" << std::endl;
        }
      } else if (received < 0) {
        std::cout << "error reading from socket" << std::endl;
      }

      if (received > 0) {
        IOLoop::Instance()->modifyHandler(fd, EPOLLOUT);
      } else {
        std::cout << "disconnect fd=" << fd << std::endl;
        IOLoop::Instance()->removeHandler(fd);
      }
    }

    return 0;
  }

private:
  int received = 0;
  char buffer[BUFFER_SIZE];
};

class ServerHandler : public Handler {
public:
  ServerHandler(int port) {
    int fd;
    sockaddr_in addr;
    memset(&addr, 0, sizeof(addr));
    
    if ((fd == socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0) {
      std::cout << "Failed to create server socket" << std::endl;
      exit(1);
    }

    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    addr.sin_port = htons(port);
  
    if (bind(fd, (sockaddr *) &addr, sizeof(addr)) < 0) {
      std::cout << "Failed to bind server socket" << std::endl;
      exit(1);
    }

    if (listen(fd, MAX_PENDING) < 0) {
      std::cout << "Failed to listen on server socket" << std::endl;
      exit(1);
    }
    setnonblocking(fd);
    
    IOLoop::Instance()->addHandler(fd, this, EPOLLIN);
  }

  virtual int handle(epoll_event e) override {
    int fd = e.data.fd;
    sockaddr_in client_addr;
    socklen_t ca_len = sizeof(client_addr);

    int client = accept(fd, (sockaddr *) &client_addr, &ca_len);
    
    if (client < 0) {
      std::cout << "Failed to accept connection" << std::endl;
      return -1;
    }

    std::cout << "accept connected: " << inet_ntoa(client_addr.sin_addr) << std::endl;
    Handler* clientHandler = new EchoHandler();
    
    IOLoop::Instance()->addHandler(client, clientHandler, EPOLLIN | EPOLLOUT | EPOLLHUP | EPOLLERR);
    return 0;
  }

private:
  void setnonblocking(int fd) {
    int flags = fcntl(fd, F_GETFL, 0);
    fcntl(fd, F_SETFL, flags | O_NONBLOCK);
  }
};


#endif // __EPOLLER_H__