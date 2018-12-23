//
// Created by roertbb on 23.12.18.
//

#ifndef KAHOOT_SERVER_CLIENTHANDLER_H
#define KAHOOT_SERVER_CLIENTHANDLER_H


#include <sys/epoll.h>
#include <sys/socket.h>
#include <unistd.h>
#include "Handler.h"
#include "Server.h"

class ClientHandler : public Handler{
    int fd;
    int epoll_fd;
public:
    ClientHandler(int fd, int epoll_fd);
    ~ClientHandler() override;
    void handleEvent(uint32_t events) override;
    int getMessageCode(char * message);

};


#endif //KAHOOT_SERVER_CLIENTHANDLER_H
