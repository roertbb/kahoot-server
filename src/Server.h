//
// Created by roertbb on 22.12.18.
//

#ifndef KAHOOT_SERVER_SERVER_H
#define KAHOOT_SERVER_SERVER_H

#include <cstdio>
#include <cstdlib>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/epoll.h>
#include <sys/poll.h>
#include "Handler.h"
#include "ClientHandler.h"

class Server {
    int server_fd;
    struct sockaddr_in server_data;

public:
    Server();
    int initSocketConnection();
    int handlePool();
};


#endif //KAHOOT_SERVER_SERVER_H
