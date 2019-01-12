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

void Client::writeMessage(int type, std::string message) {
    std::string typeToStr = std::to_string(type);
    std::string parsedMessageType = std::string(2 - typeToStr.length(),'0').append(typeToStr) + "|";
    std::string messageToSize = std::to_string(message.length() + 3);
    std::string messageSize = std::string(4 - messageToSize.length(), '0').append(messageToSize);
    char * c = const_cast<char*>((messageSize).c_str());
    //std::cout << messageSize << std::endl;
    if ((write(this->fd,c,4)) == -1) {
        perror("sending message size failed");
    }
    //std::cout << (parsedMessageType + message) << std::endl;
    char * c2 = const_cast<char*>((parsedMessageType + message).c_str());
    if ((write(this->fd,c2,(parsedMessageType + message).length())) == -1) {
        perror("sending message failed");
    }
}
