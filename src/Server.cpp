//
// Created by roertbb on 22.12.18.
//

#include "Server.h"

Server::Server() {
    this->initSocketConnection();
    this->handlePoll();
}

int Server::initSocketConnection() {

    this->server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (this->server_fd < 0) {
        perror("Creating socket failed\n");
        return 1;
    }

    int n = 1;
    setsockopt(this->server_fd, SOL_SOCKET, SO_REUSEADDR, &n, sizeof(n));

    // set non-blocking mode
    fcntl(this->server_fd, F_SETFL, O_NONBLOCK);

    // TODO: create config file with server data
    sockaddr_in server_data {
        .sin_family = AF_INET,
        .sin_port = htons(1234),
        .sin_addr = {inet_addr("127.0.0.1")}
    };

    if ((bind(this->server_fd, (sockaddr *) &server_data, sizeof(server_data))) < 0) {
        perror("Binding socket failed\n");
        return 1;
    }

    if ((listen(this->server_fd, 10)) < 0) {
        perror("Creating queue for listening sockets failed\n");
        return 1;
    }

    printf("Listening for client's connections\n");
}

int Server::handlePoll() {
    this->epoll_fd = epoll_create1(0);

    epoll_event ee {EPOLLIN, {.fd = this->server_fd}};
    epoll_ctl(this->epoll_fd, EPOLL_CTL_ADD, this->server_fd, &ee);

    while(true) {
        if ((epoll_wait(this->epoll_fd, &ee, 1, -1)) == -1) {
            perror("Receiving message to epoll failed\n");
            return 1;
        }
        // handle new user
        if (ee.events & EPOLLIN && ee.data.fd == server_fd) {
            sockaddr_in client_address {};
            socklen_t client_address_size = sizeof(client_address);

            int client_fd = accept(server_fd, (sockaddr *) &client_address, &client_address_size);
            if (client_fd < 0) {
                perror("Accepting client failed");
                return 1;
            }

            this->clients.insert(new Client(client_fd, this->epoll_fd));

            printf("new connection from: %s:%hu (fd: %d)\n", inet_ntoa(client_address.sin_addr), ntohs(client_address.sin_port), client_fd);
        }
        else {
            // handle existing user - loop over clients and handle their requests, if so read data and handle their request
            for (Client * client : this->clients) {
                if (ee.events & EPOLLIN && ee.data.fd == client->getFd()) {
                    char buffer[1024]="";
                    int count = read(client->getFd(), buffer, 1024);
                    if (count > 0) {
                        this->handleClient(client, buffer);
                    }
                }
                if(ee.events & ~EPOLLIN && ee.data.fd == client->getFd()){
                    this->clients.erase(client);
                    delete client;
                }
            }
            for (Kahoot * kahoot : this->kahoots) {
                int timerFd = kahoot->getTimerFd();
                if (ee.events & EPOLLIN && ee.data.fd == timerFd) {
                    printf("timer with fd: %d\n", timerFd);
                    // clear timer
                    epoll_ctl(this->epoll_fd,EPOLL_CTL_DEL,timerFd,NULL);
                    close(timerFd);
                    // call kahoot to make next move
                    kahoot->next();
                }
            }
        }
    }
}

int Server::writeMessage(Client *client, std::string message) {
    char * c = const_cast<char*>(message.c_str());
    if ((write(client->getFd(),c,message.length())) == -1){
        perror("sending message failed");
        return 1;
    }
}

int Server::getMessageCode(char *buffer) {
    int num1 = buffer[0] - 48;
    int num2 = buffer[1] - 48;
    return num1*10 + num2;
}

int Server::handleClient(Client *client, char * buffer) {
    int msgcode = getMessageCode(buffer);
    switch(msgcode) {
        case 1:
            printf("user with fd: %d - disconnected\n",client->getFd());
            this->clients.erase(client);
            delete client;
            break;
        case 2:
            this->createKahoot(buffer, client);
            break;
        case 3:
            this->sendRooms(client);
            break;
        case 4:
            this->addToRoom(buffer, client);
            break;
        case 5:
            client->getParticipatingIn()->next();
            break;
        case 6:
            client->getParticipatingIn()->receiveAnswer(client,buffer);
            break;
    }
}

void Server::createKahoot(char *data, Client * owner) {
    Kahoot * kahoot = new Kahoot(owner, data, this->generateUniqueId(), this->epoll_fd);
    this->kahoots.insert(kahoot);
}

int Server::sendRooms(Client *client) {
    printf("sending rooms data to fd:%d\n", client->getFd());
    std::string data = "03|";
    for (Kahoot * k : this->kahoots) {
        data += std::to_string(k->getId()) + "|";
    }
    this->writeMessage(client,data);
}

int Server::generateUniqueId() {
    int id = rand() % 10000;
    bool unique = false;
    while(!unique) {
        unique = true;
        for (Kahoot * k : this->kahoots) {
            if (k->getId() == id)
                unique = false;
        }
    }
    return id;
}

int Server::addToRoom(char *buffer, Client *client) {
    char * ptr = strtok(buffer,"|");
    // skip 1st value indicating communicate type
    ptr = strtok(NULL, "|");
    int roomId = atoi(ptr);
    ptr = strtok(NULL, "|");
    int pin = atoi(ptr);
    ptr = strtok(NULL,"|");
    std::string nick = std::string(ptr);

    // check if pin is correct
    for (Kahoot * k : this->kahoots) {
        if (k->getId() == roomId) {
            if (k->getPin() == pin){
                client->setNick(nick);
                client->setParticipatingIn(k);
                k->addPlayer(client);
                this->writeMessage(client,"04|success|");
                this->broadcastPlayers(k);
            }
            else {
                this->writeMessage(client,"04|incorrect pin|");
            }
        }
    }
}

//TODO: move to Kahoot (?)
int Server::broadcastPlayers(Kahoot *kahoot) {
    std::string playersInRoom = "05|";
    for(auto const& [key, val] : kahoot->getPlayers()) {
        playersInRoom += key->getNick() + "|";
    }
    char * c = const_cast<char*>(playersInRoom.c_str());
    // send message to channels owner
    printf("broadcasting to: %d\n", kahoot->getOwner()->getFd());
    if ((write(kahoot->getOwner()->getFd(),c,playersInRoom.length())) == -1){
        perror("broadcasting users in room data failed");
        return 1;
    }
    // send message to other players in room
    for(auto const& [key, val] : kahoot->getPlayers()) {
        printf("broadcasting to: %d\n", key->getFd());
        if ((write(key->getFd(),c,playersInRoom.length())) == -1){
            perror("broadcasting users in room data failed");
            return 1;
        }
    }
}
