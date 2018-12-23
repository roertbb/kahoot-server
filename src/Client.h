//
// Created by roertbb on 23.12.18.
//

#ifndef KAHOOT_SERVER_CLIENT_H
#define KAHOOT_SERVER_CLIENT_H


#include <sys/epoll.h>
#include <sys/socket.h>
#include <unistd.h>
#include <cstdio>

class Client{
    int fd;
    int epoll_fd;
public:
    Client(int fd, int epoll_fd);
    ~Client();
    int getFd();

//    void handleEvent(uint32_t events) override;
//    int getMessageCode(char * message);

};


#endif //KAHOOT_SERVER_CLIENT_H
