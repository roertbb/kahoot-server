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

class Server {
    int socket_desc;
    struct sockaddr_in server_data;

public:
    Server();
    int initSocketConnection();

};


#endif //KAHOOT_SERVER_SERVER_H
