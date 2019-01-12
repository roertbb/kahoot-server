//
// Created by roertbb on 22.12.18.
//

#ifndef KAHOOT_SERVER_SERVER_H
#define KAHOOT_SERVER_SERVER_H

#define USER_DISCONNECTED 1
#define CREATE_KAHOOT 2
#define SEND_ROOMS 3
#define JOIN_ROOM 4
#define SEND_PLAYERS_IN_ROOM 5
#define OWNER_START_KAHOOT 5
#define START_KAHOOT 6
#define RECEIVE_ANSWER_FROM_CLIENT 6
#define SEND_QUESTION 7
#define SEND_ANSWER_CORRECT 8
#define SEND_PREPARE_BEFORE_NEXT 9
#define SEND_ANSWER_TO_HOST 10
#define REQUEST_ROOM_MEMBERS 11
#define SEND_OWNER_DISCONNECTED 12
#define CHECK_IF_ALREADY_STARTED 13

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
#include <cerrno>
#include <error.h>
#include <sstream>
#include <cstring>
#include <sys/timerfd.h>
#include "Client.h"
#include "Kahoot.h"


class Server {
    int server_fd;
    int epoll_fd;
    struct sockaddr_in server_data;
    std::unordered_set<Client*> clients;
    std::unordered_set<Kahoot*> kahoots;
public:
    void run();
    int initSocketConnection();
    int handlePoll();
    int handleClient(Client * client, char * buffer);
    int getMessageCode(char * buffer);
    void createKahoot(char * data, Client * owner);
    int sendRooms(Client * client);
    int generateUniqueId();
    int addToRoom(char * buffer, Client * client);
};


#endif //KAHOOT_SERVER_SERVER_H
