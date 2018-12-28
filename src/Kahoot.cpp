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
        for(auto const& [key, val] : this->getPlayers()) {
            this->writeMessage(key, "06|");
        }
        this->setTimer();
        return 0;
    } else if (this->state == "prep-question") {
        this->state = "question";
        this->writeMessage(this->owner, "07|send question");
        for(auto const& [key, val] : this->getPlayers()) {
            this->writeMessage(key, "07|"+this->questions[this->currentQuestion]);
        }
        this->setTimer();
        return 0;
    } else if (this->state == "question") {
        this->state = "answers";
        //TODO: if last answer send full fledged results
        this->writeMessage(this->owner, "08|send answers");
        for(auto const& [key, val] : this->getPlayers()) {
            int received = this->receivedAnswers[key];
            std::string answer;
            if (received == 1)
                answer = "correct";
            else if (received == 0)
                answer = "incorrect";
            else
                answer = "answer not found";
            this->writeMessage(key, "08|" + answer);
        }
        // clear answers map
        this->receivedAnswers.clear();
        this->setTimer();
        return 0;
    } else if (this->state == "answers") {
        if (this->currentQuestion == this->questions.size()-1) {
            // last question - do clean up - perhaps return -1 in order to notify server that it was the last question
            this->owner->setParticipatingIn(nullptr);
            for(auto const& [key, val] : this->getPlayers()) {
                key->setParticipatingIn(nullptr);
            }
            return -1;
        } else {
            this->currentQuestion++;
            this->state = "prep-question";
            this->writeMessage(this->owner, "09|prepare for next question");
            for(auto const& [key, val] : this->getPlayers()) {
                this->writeMessage(key, "09|");
            }
            this->setTimer();
            return 0;
        }
    }
}

int Kahoot::writeMessage(Client *client, std::string message) {
    char * c = const_cast<char*>(message.c_str());
    if ((write(client->getFd(),c,message.length())) == -1) {
        perror("sending message failed");
        return 1;
    }
}

int Kahoot::receiveAnswer(Client *client, char *buffer) {
    char * ptr = strtok(buffer, "|");
    ptr = strtok(NULL, "|");
    if (!strcmp(ptr,this->answers[this->currentQuestion].c_str())) {
        // mark that user answered question correctly
        this->receivedAnswers.insert(std::pair<Client*,int>(client,1));
        // calc point according to time
        itimerspec timer_data;
        if ((timerfd_gettime(this->timer_fd, &timer_data)) == -1) {
            perror("reading timer data failed\n");
            return 1;
        }
        int points = 1000 * (timer_data.it_value.tv_sec * 1000 + timer_data.it_value.tv_nsec) / (this->times[this->currentQuestion] * 1000);
        this->players[client] = this->players[client] + points;
    }
    else {
        // mark that user answered incorrectly
        this->receivedAnswers.insert(std::pair<Client*,int>(client,0));
    }
}