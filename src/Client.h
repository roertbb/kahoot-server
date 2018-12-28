//
// Created by roertbb on 23.12.18.
//

#ifndef KAHOOT_SERVER_CLIENT_H
#define KAHOOT_SERVER_CLIENT_H


#include <sys/epoll.h>
#include <sys/socket.h>
#include <unistd.h>
#include <cstdio>
#include <string>

// forward declaration
class Kahoot;

class Client{
    int fd;
    int epoll_fd;
    std::string nick;
    Kahoot * participatingIn;
public:
    Client(int fd, int epoll_fd);
    ~Client();
    int getFd();
    void setNick(std::string nick);
    std::string getNick();
    void setParticipatingIn(Kahoot * kahoot);
    Kahoot * getParticipatingIn();
};


#endif //KAHOOT_SERVER_CLIENT_H
