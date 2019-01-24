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
#include "Buffer.h"

// forward declaration
class Kahoot;

class Client{
    int fd;
    int epoll_fd;
    std::string nick;
    Kahoot * participatingIn;
    Buffer toWrite;
public:
    Client(int fd, int epoll_fd);
    ~Client();
    int getFd();
    void setNick(std::string nick);
    std::string getNick();
    void setParticipatingIn(Kahoot * kahoot);
    Kahoot * getParticipatingIn();
    void writeMessage(int type, std::string message);
    int writeRemaining();
    void toggleWrite(bool write);
};


#endif //KAHOOT_SERVER_CLIENT_H
