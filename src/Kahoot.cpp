//
// Created by roertbb on 23.12.18.
//

#include <sys/timerfd.h>
#include <algorithm>
#include <iostream>
#include "Kahoot.h"

Kahoot::Kahoot(Client *owner, char *question_data, int id, int epoll_fd) {
    this->id = id;
    this->owner = owner;
    owner->setParticipatingIn(this);
    this->timer_fd = 0;
    this->epoll_fd = epoll_fd;
    this->currentQuestion = 0;
    this->state="not-started";
    // parse question_data
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
    this->points.push_back(std::pair<std::string,int>(client->getNick(),0));
    this->connectedPlayers.push_back(client);
}

Client *Kahoot::getOwner() {
    return this->owner;
}

int Kahoot::getTimerFd() {
    return this->timer_fd;
}

void Kahoot::setTimer() {

    int wait_time = breaktime;
    if (this->state == "question") {
        wait_time = this->times[this->currentQuestion];
    }
    // create timer
    int timer_fd = timerfd_create(CLOCK_REALTIME, 0);
    itimerspec ts {
            .it_interval = {
                    .tv_sec = 0,
                    .tv_nsec = 0
            },
            .it_value = {
                    .tv_sec = wait_time,
                    .tv_nsec = 0
            }
    };
    // register epoll event
    epoll_event ee {EPOLLIN, {.fd = timer_fd}};
    epoll_ctl(epoll_fd, EPOLL_CTL_ADD, timer_fd, &ee);
    // start timer
    timerfd_settime(timer_fd, 0, &ts, 0);
    this->timer_fd = timer_fd;
}

int Kahoot::next() {
    if (this->state == "not-started") {
        this->state = "prep-question";
        this->writeMessage(this->owner, "06|start kahoot");
        for(Client * client : this->connectedPlayers) {
            this->writeMessage(client, "06|");
        }
        this->setTimer();
        return 0;
    } else if (this->state == "prep-question") {
        this->state = "question";
        this->writeMessage(this->owner, "07|send question");
        for(Client * client : this->connectedPlayers) {
            this->writeMessage(client, "07|"+this->questions[this->currentQuestion]);
        }
        this->setTimer();
        return 0;
    } else if (this->state == "question") {
        this->state = "answers";
        this->writeMessage(this->owner, "08|");
        // summarize answers in order to send clients summary of current question
        std::map<std::string,int> ans;
        for (auto const& a : this->receivedAnswers) {
            ans[a.second] = ans[a.second]+1;
        }
        std::string answersStr;
        for (auto const& a : ans) {
            answersStr += "|" + a.first + ":" + std::to_string(a.second);
        }
        for (Client * client : this->connectedPlayers) {
            std::string received = this->receivedAnswers[client];
            std::string answer;
            if (received == this->answers[this->currentQuestion])
                answer = "1";
            else if (received != this->answers[this->currentQuestion])
                answer = "0";
            else
                answer = "2";
            this->writeMessage(client, "08|" + answer + "|" + this->answers[this->currentQuestion] + answersStr + "|");
        }
        // clear answers map
        this->receivedAnswers.clear();
        this->setTimer();
        return 0;
    } else if (this->state == "answers") {
        if (this->currentQuestion == this->questions.size()-1) {
            // last question - do clean up - perhaps return -1 in order to notify server that it was the last question
            this->writeMessage(this->owner, "09|0|");
            this->owner->setParticipatingIn(nullptr);
            for (Client * client : this->connectedPlayers) {
                // send message to user with score and placement
                for (int i=0; i<this->points.size(); i++) {
                    if (this->points[i].first == client->getNick()) {
                        this->writeMessage(client, "09|0|" + std::to_string(i+1) + "|" +std::to_string(this->points[i].second));
                        break;
                    }
                }
                client->setParticipatingIn(nullptr);
            }
            return -1;
        } else {
            this->currentQuestion++;
            this->state = "prep-question";
            this->writeMessage(this->owner, "09|");
            for (Client * client : this->connectedPlayers) {
                this->writeMessage(client, "09|");
            }
            this->setTimer();
            return 0;
        }
    }
}

int Kahoot::writeMessage(Client *client, std::string message) {
    std::string msgToSize = std::to_string(message.length());
    std::string msgSize = std::string(4 - msgToSize.length(), '0').append(msgToSize);
    char * c = const_cast<char*>(msgSize.c_str());
    if ((write(client->getFd(),c,4)) == -1) {
        perror("sending message size failed");
        return 1;
    }
    char * c2 = const_cast<char*>(message.c_str());
    if ((write(client->getFd(),c2,message.length())) == -1) {
        perror("sending message failed");
        return 1;
    }
}

int Kahoot::receiveAnswer(Client *client, char *buffer) {
    //TODO: skip msgcode when receiving message;
    char * ptr = strtok(buffer, "|");
    ptr = strtok(NULL, "|");

    bool isAnswerCorrect = !strcmp(ptr,this->answers[this->currentQuestion].c_str());
    // if user respond correctly we will construct string with current users score
    std::string usersScore = "";
    // save user answer
    this->receivedAnswers.insert(std::pair<Client*,std::string>(client,ptr));
    // calculate time
    float remainingTime = this->getRemainingTime();
    float answerTime = (float) this->times[this->currentQuestion] - remainingTime;

    if (isAnswerCorrect) {
        // calculate points
        int points = (int) (1000.0 * remainingTime / (float) this->times[this->currentQuestion]);
        // assign value to user
        for(int i=0; i<this->points.size(); i++) {
            if (this->points[i].first == client->getNick()) {
                this->points[i].second = this->points[i].second + points;
            }
        }
        // sort points
        std::sort(this->points.begin(),this->points.end(),[](std::pair<std::string, int> a, std::pair<std::string, int> b) {return a.second > b.second; });
        // prepare user score
        for (std::pair<std::string,int> p : this->points) {
            usersScore += "|" + p.first + " - " + std::to_string(p.second);
        }
    }

    this->writeMessage(this->owner,"10|" + client->getNick() + "|" + std::string(ptr) + "|" + std::to_string(answerTime) + usersScore + "|");

    // check if all users answered, if so remove timer and go to next state
    if (this->receivedAnswers.size() == this->connectedPlayers.size()) {
        epoll_ctl(this->epoll_fd,EPOLL_CTL_DEL,this->timer_fd,NULL);
        close(this->timer_fd);
        this->next();
    }
}

void Kahoot::sendPlayersInRoom(Client * client) {
    std::string playersInRoom = "05|";
    for (Client * client : this->connectedPlayers) {
        playersInRoom += client->getNick() + "|";
    }
    // if client is not defined broadcast message to all participating users
    if (client == nullptr) {
        this->writeMessage(this->owner,playersInRoom);
        for (Client * client : this->connectedPlayers) {
            this->writeMessage(client, playersInRoom);
        }
    }
    else {
        // broadcast players
        this->writeMessage(client,playersInRoom);
        // check if kahoot already started, if so join it
        if (this->getState() != "not-started")
            this->writeMessage(client, "06|");
    }
}

float Kahoot::getRemainingTime() {
    itimerspec timer_data;
    if ((timerfd_gettime(this->timer_fd, &timer_data)) == -1) {
        perror("reading timer data failed\n");
        return 1;
    }
    return (float) timer_data.it_value.tv_sec + (float) timer_data.it_value.tv_nsec / 1000000000.0;
}

void Kahoot::removePlayer(Client *client) {
    for (int i=0; i<this->connectedPlayers.size(); i++) {
        if (this->connectedPlayers[i] == client)
            this->connectedPlayers.erase(this->connectedPlayers.begin()+i);
    }
}

void Kahoot::ownerDisconnected() {
    if (this->state == "not-started") {
        for (Client * client : this->connectedPlayers) {
            this->writeMessage(client,"12|");
        }
    }
}

std::string Kahoot::getState() {
    return this->state;
}
