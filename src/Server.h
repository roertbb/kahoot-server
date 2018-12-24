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
#include <unordered_set>
#include "Client.h"
#include "Kahoot.h"


class Server {
    int server_fd;
    struct sockaddr_in server_data;
    std::unordered_set<Client*> clients;
    std::unordered_set<Kahoot*> kahoots;

public:
    Server();
    int initSocketConnection();
    int handlePool();
    int handleClient(Client * client, char * buffer);
    int getMessageCode(char * buffer);
    void createKahoot(char * data, Client * owner);
};


#endif //KAHOOT_SERVER_SERVER_H
