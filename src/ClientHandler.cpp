//
// Created by roertbb on 23.12.18.
//

#include <cstdio>
#include "ClientHandler.h"


ClientHandler::ClientHandler(int fd, int epoll_fd) {
    this->fd = fd;
    this->epoll_fd = epoll_fd;
    epoll_event ee {EPOLLIN|EPOLLRDHUP, {.ptr=this}};
    epoll_ctl(epoll_fd, EPOLL_CTL_ADD, fd, &ee);
}

ClientHandler::~ClientHandler() {
    epoll_ctl(this->epoll_fd, EPOLL_CTL_DEL, fd, nullptr);
    shutdown(this->fd, SHUT_RDWR);
    close(this->fd);
}

void ClientHandler::handleEvent(uint32_t events) {
    if (events & EPOLLIN) {
        char buffer[256];
        int count = read(this->fd, buffer, 256);
        if (count > 0) {
            //handle event
            printf("%s\n", buffer);
        }
        else {
            events |= EPOLLERR;
        }
    }
    if (events & ~ EPOLLIN) {
        //TODO: remove, erase from set, close fd, delete this
    }
}
