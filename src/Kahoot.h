//
// Created by roertbb on 23.12.18.
//

#ifndef KAHOOT_SERVER_KAHOOT_H
#define KAHOOT_SERVER_KAHOOT_H

#include <vector>
#include <map>
#include <cstring>
#include <sstream>
#include "Client.h"

static int breaktime = 5;

class Kahoot {
    int id;
    int pin;
    Client * owner;
    std::vector<Client*> connectedPlayers;
    std::vector<std::pair<std::string,int>> points;
    std::vector<std::string> questions;
    std::vector<std::string> answers;
    std::vector<int> times;
    std::map<Client*,std::string> receivedAnswers;
    int timer_fd;
    int epoll_fd;
    int currentQuestion;
    std::string state;
public:
    Kahoot(Client * owner, char * question_data, int id, int epoll_fd);
    int getId();
    int getPin();
    void addPlayer(Client * client);
    Client* getOwner();
    int getTimerFd();
    int next();
    void setTimer();
    int writeMessage(Client * client, std::string message);
    int receiveAnswer(Client * client, char * buffer);
    void broadcastPlayersInRoom();
    float getRemainingTime();
};


#endif //KAHOOT_SERVER_KAHOOT_H
