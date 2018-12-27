//
// Created by roertbb on 23.12.18.
//

#include <cstring>
#include "Kahoot.h"

Kahoot::Kahoot(Client *owner, char *question_data, int id) {
    this->started = false;
    this->id = id;
    this->owner = owner;
    // parse question_data to questions...
    int count_separators = 0;
    int begin_question = 3;
    for (int i=3; question_data[i]!='\0'; i++) {
        if (question_data[i] == '|') {
            count_separators++;
            //save question
            if (count_separators == 7) {
                int size = i-begin_question+1;
                char * question = new char[size];
                strncpy(question, question_data+begin_question, size);
                this->questions.push_back(question);
                begin_question = i+1;
                count_separators = 0;
            }
        }
    }
    // ...answers and time
    for (char * c : this->questions) {
        int pipes = 0;
        int num = 0;
        for (int i=0; c[i]!='\0'; i++) {
            if (c[i] == '|') {
                pipes++;
                // get answer
                if (pipes == 5) {
                    this->answers.push_back(c[i+1]);
                }
                // ...and save number
                if (pipes == 7) {
                    this->times.push_back(num);
                    num = 0;
                }
            }
            // collect numbers when it's time for it...
            if (pipes == 6 && c[i+1]!='|') {
                num = num*10 + (c[i+1] - 48);
            }
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




