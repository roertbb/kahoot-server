//
// Created by roertbb on 23.12.18.
//

#include <sys/timerfd.h>
#include "Kahoot.h"

Kahoot::Kahoot(Client *owner, char *question_data, int id, int epoll_fd) {
    this->id = id;
    this->owner = owner;
    owner->setParticipatingIn(this);
    this->timer_fd = 0;
    this->epoll_fd = epoll_fd;
    this->currentQuestion = 1;
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
    this->players.insert(std::pair<Client*,int>(client,0));
}

std::map<Client*, int> Kahoot::getPlayers() {
    return this->players;
}

Client *Kahoot::getOwner() {
    return this->owner;
}

int Kahoot::getTimerFd() {
    return this->timer_fd;
}

void Kahoot::start() {
    // broadcast messages - host
    // broadcast messages - first question for clients
    // define timer
    this->setTimer();
}

void Kahoot::setTimer() {
    // create timer
    int timer_fd = timerfd_create(CLOCK_REALTIME, 0);
    itimerspec ts {
            .it_interval = {
                    .tv_sec = 0,
                    .tv_nsec = 0
            },
            .it_value = {
                    .tv_sec = breaktime,
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

void Kahoot::next() {

}




