//
// Created by roertbb on 23.12.18.
//

#ifndef KAHOOT_SERVER_KAHOOT_H
#define KAHOOT_SERVER_KAHOOT_H

#include "Client.h"
#include <vector>
#include <map>

class Kahoot {
    Client * owner;
    std::map<Client,int> players;
    std::vector<char*> questions;
    std::vector<char> answers;
    std::vector<int> times;
public:
    Kahoot(Client * owner, char * question_data);
};


#endif //KAHOOT_SERVER_KAHOOT_H
