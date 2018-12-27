//
// Created by roertbb on 23.12.18.
//

#include "Kahoot.h"

Kahoot::Kahoot(Client *owner, char *question_data, int id) {
    this->started = false;
    this->id = id;
    this->owner = owner;
    char * ptr = strtok(question_data, "|");
    // skip 1st value indicating communicate type
    while((ptr = strtok(NULL, "|")) != NULL) {
        this->questions.push_back(std::string(ptr));
    }
    // iterate over questions in order to obtain answers and time
    for (std::string question : this->questions) {
        std::string token;
        std::istringstream questionStream(question);
        for (int i=0; std::getline(questionStream, token, '#'); i++) {
            if (i==5)
                this->answers.push_back(token);
            if (i==6)
                this->times.push_back(std::stoi(token));
        }
    }
    // generate pin
    this->pin = rand() % 10000;
    printf("Created kahoot with id: %d with pin: %d\n", this->id, this->pin);
}

int Kahoot::getId() {
    return this->id;
}

int Kahoot::getPin() {
    return this->pin;
}

void Kahoot::addPlayer(Client *client) {
    this->players.insert(std::pair<Client*,int>(client,0));
}

std::map<Client*, int> Kahoot::getPlayers() {
    return this->players;
}

Client *Kahoot::getOwner() {
    return this->owner;
}




