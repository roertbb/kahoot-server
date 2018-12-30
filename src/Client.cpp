//
// Created by roertbb on 23.12.18.
//

#include "Client.h"
#include "Kahoot.h"

Client::Client(int fd, int epoll_fd) {
    this->fd = fd;
    this->epoll_fd = epoll_fd;
    epoll_event ee {EPOLLIN|EPOLLRDHUP, {.fd=this->fd}};
    epoll_ctl(epoll_fd, EPOLL_CTL_ADD, fd, &ee);
}

Client::~Client() {
    Kahoot * myKahoot = this->participatingIn;
    if (myKahoot != nullptr) {
        if (myKahoot->getOwner() == this)
            myKahoot->ownerDisconnected();
        else
            myKahoot->removePlayer(this);
    }
    epoll_ctl(this->epoll_fd, EPOLL_CTL_DEL, fd, nullptr);
    shutdown(this->fd, SHUT_RDWR);
    close(this->fd);
}

int Client::getFd() {
    return this->fd;
}

void Client::setNick(std::string nick) {
    this->nick = nick;
}

std::string Client::getNick() {
    return this->nick;
}

void Client::setParticipatingIn(Kahoot *kahoot) {
    this->participatingIn = kahoot;
}

Kahoot *Client::getParticipatingIn() {
    return this->participatingIn;
}
