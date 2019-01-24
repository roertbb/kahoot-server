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

void Client::writeMessage(int type, std::string message) {
    std::string typeToStr = std::to_string(type);
    std::string parsedMessageType = std::string(2 - typeToStr.length(),'0').append(typeToStr) + "|";
    std::string parsedMessage = parsedMessageType + message + "\n";
    int msgSize = parsedMessage.size();

    char * c = const_cast<char*>((parsedMessage).c_str());

    // if data already in buffer, add it at the end of the buffer
    if (this->toWrite.pending) {
        this->toWrite.appendToBuffer(c,msgSize);
        return;
    }
    // else try to send it to user
    else {
        int sent = send(this->fd, c, msgSize, MSG_DONTWAIT);
        // if whole message send - all is ok - return
        if (sent == msgSize) {
            return;
        }
        // error handling
        else if (sent <= 0) {
            this->toWrite.writeLater(c,msgSize);
        }
        // if not whole add it to buffer
        else {
            this->toWrite.writeLater(c+sent,msgSize-sent);
        }
        // toggle buffer to send data when possible
        this->toggleWrite(true);
    }
}

int Client::writeRemaining() {
    int rem = this->toWrite.remaining();
    int sent = send(this->fd, this->toWrite.data+this->toWrite.pos, rem,MSG_DONTWAIT);
    if (sent == rem) {
        this->toWrite.pending = false;
        this->toggleWrite(false);
    }
    else if (sent == -1 && errno != EWOULDBLOCK && errno != EAGAIN) {
        error(0,errno,"Sending data failed");
        return -1;
    }
    else
        this->toWrite.pos += sent;
    return 0;
}

void Client::toggleWrite(bool write) {
    unsigned int flags = EPOLLIN|EPOLLRDHUP;
    if (write)
        flags |= EPOLLOUT;
    epoll_event ee {flags, {.fd=this->fd}};
    epoll_ctl(this->epoll_fd, EPOLL_CTL_MOD, this->fd, &ee);
}
