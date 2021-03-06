//
// Created by roertbb on 22.12.18.
//

#include <iostream>
#include <fstream>
#include "Server.h"

void Server::run() {
    this->initSocketConnection();
    this->handlePoll();
}

void Server::initSocketConnection() {
    //read config data
    std::ifstream configFile;
    configFile.open(".env");
    std::string serverAddress, serverPort;
    if (!configFile.is_open())
        error(1, errno, "Error reading config file");

    configFile >> serverAddress;
    configFile >> serverPort;
    configFile.close();

    this->server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (this->server_fd < 0)
        error(1, errno, "Creating socket failed");


    int n = 1;
    setsockopt(this->server_fd, SOL_SOCKET, SO_REUSEADDR, &n, sizeof(n));

    // set non-blocking mode
    fcntl(this->server_fd, F_SETFL, O_NONBLOCK);

    sockaddr_in server_data {};
    server_data.sin_family = AF_INET;
    server_data.sin_port = htons(std::stoi(serverPort));
    server_data.sin_addr = {INADDR_ANY}; //accept connection to all the IPs of the machine


    if ((bind(this->server_fd, (sockaddr *) &server_data, sizeof(server_data))) < 0)
        error(1, errno, "Binding socket failed");


    if ((listen(this->server_fd, 10)) < 0)
        error(1, errno, "Creating queue for listening sockets failed");

    printf("Listening for client's connections\n");
}

void Server::handlePoll() {
    this->epoll_fd = epoll_create1(0);

    epoll_event ee {EPOLLIN, {.fd = this->server_fd}};
    epoll_ctl(this->epoll_fd, EPOLL_CTL_ADD, this->server_fd, &ee);

    while(true) {
        if ((epoll_wait(this->epoll_fd, &ee, 1, -1)) == -1)
            error(0, errno, "epoll failed");

        // handle new user
        if (ee.events & EPOLLIN && ee.data.fd == server_fd) {
            sockaddr_in client_address {};
            socklen_t client_address_size = sizeof(client_address);

            int client_fd = accept(server_fd, (sockaddr *) &client_address, &client_address_size);
            if (client_fd < 0)
                error(1, errno, "Accepting client failed");

            //set no-blocking mode
            fcntl(client_fd, F_SETFL, O_NONBLOCK);

            this->clients.insert(new Client(client_fd, this->epoll_fd));

            printf("new connection from: %s:%hu (fd: %d)\n", inet_ntoa(client_address.sin_addr), ntohs(client_address.sin_port), client_fd);
        }
        else {
            // handle existing user - loop over clients and handle their requests, if so read data and handle their request
            for (Client * client : this->clients) {
                int clientFd = client->getFd();

                // first send remaining data
                if (ee.events & EPOLLOUT && ee.data.fd == clientFd) {
                    if (client->writeRemaining() < 0)
                        ee.events |= EPOLLERR;
                }
                // then accept new
                else if (ee.events & EPOLLIN && ee.data.fd == clientFd) {
                    int count = recv(clientFd, client->receiver.dataPos(),client->receiver.remaining(),0);
                    if (count <= 0) {
                        error(0, errno, "Receiving message from client failed");
                        ee.events |= EPOLLERR;
                    }
                    else {
                        client->receiver.pos += count;
                        char * endOfMessage = (char*) memchr(client->receiver.data, '\n', client->receiver.pos);
                        if (endOfMessage == nullptr && !client->receiver.remaining())
                            client->receiver.resize();
                        else {
                            do {
                                int currentMessageLen = endOfMessage - client->receiver.data +1;
                                char * msg = new char[currentMessageLen];
                                strncpy(msg,client->receiver.data,currentMessageLen-1);
                                //std::cout << msg << std::endl;
                                this->handleClient(client, msg);
                                delete [] msg;
                                int nextMessageBegin = client->receiver.pos - currentMessageLen;
                                memmove(client->receiver.data, endOfMessage+1, nextMessageBegin);
                                client->receiver.pos = nextMessageBegin;
                            } while((endOfMessage = (char*) memchr(client->receiver.data, '\n', client->receiver.pos)));
                        }
                    }
                }
                // disconnect client when error occurred
                if (ee.events & ~(EPOLLIN|EPOLLOUT) && ee.data.fd == clientFd) {
                    this->deleteClient(client);
                }
            }
            for (auto k: this->kahoots) {
                int timerFd = k.second->getTimerFd();
                if (ee.events & EPOLLIN && ee.data.fd == timerFd) {
                    // clear timer
                    epoll_ctl(this->epoll_fd,EPOLL_CTL_DEL,timerFd,NULL);
                    close(timerFd);
                    // call kahoot to make next move
                    if ((k.second->next()) == -1) {
                        // delete kahoot
                        this->deleteKahoot(k);
                    }
                }
            }
        }
    }
}

int Server::getMessageCode(char *buffer) {
    int num1 = buffer[0] - 48;
    int num2 = buffer[1] - 48;
    return num1*10 + num2;
}

void Server::handleClient(Client *client, char * buffer) {
    int msgcode = getMessageCode(buffer);
    switch(msgcode) {
        case USER_DISCONNECTED:
            printf("user with fd: %d - disconnected\n",client->getFd());
            this->deleteClient(client);
            break;
        case CREATE_KAHOOT:
            this->createKahoot(buffer, client);
            break;
        case SEND_ROOMS:
            this->sendRooms(client);
            break;
        case JOIN_ROOM:
            this->addToRoom(buffer, client);
            break;
        case OWNER_START_KAHOOT:
            client->getParticipatingIn()->next();
            break;
        case RECEIVE_ANSWER_FROM_CLIENT:
            client->getParticipatingIn()->receiveAnswer(client,buffer);
            break;
        case REQUEST_ROOM_MEMBERS:
            client->getParticipatingIn()->sendPlayersInRoom(client);
            break;
        case CHECK_IF_ALREADY_STARTED:
            client->getParticipatingIn()->checkIfAlreadyStarted(client);
            break;
        case GET_PIN:
            client->getParticipatingIn()->sendPin();
            break;
        default:
            printf("%s\n",buffer);
            perror("Should never happen...\n");
    }
}

void Server::createKahoot(char *data, Client * owner) {
    int id = this->generateUniqueId();
    Kahoot * kahoot = new Kahoot(owner, data, id, this->epoll_fd);
    this->kahoots[id] = kahoot;
    // notify all users that new kahoot was created
    this->sendRooms(nullptr);
}

void Server::sendRooms(Client * client) {
    std::string data = "";
    for (auto k : this->kahoots) {
        data += std::to_string(k.second->getId()) + "|";
    }
    // if client is not defined broadcast message to all users
    if (client == nullptr) {
        for (Client * c : this->clients) {
            c->writeMessage(SEND_ROOMS,data);
        }
    }
    else
        client->writeMessage(SEND_ROOMS,data);
}

int Server::generateUniqueId() {
    bool unique;
    int id;
    do {
        unique = true;
        id = rand() % 10000;
        for (auto k : this->kahoots) {
            if (k.second->getId() == id)
                unique = false;
        }
    } while(!unique);
    return id;
}

void Server::addToRoom(char *buffer, Client *client) {
    char * ptr = strtok(buffer,"|");
    // skip 1st value indicating communicate type
    ptr = strtok(NULL, "|");
    int roomId = atoi(ptr);
    ptr = strtok(NULL, "|");
    int pin = atoi(ptr);
    ptr = strtok(NULL,"|");
    std::string nick = std::string(ptr);

    // check if pin is correct
    for (auto k : this->kahoots) {
        if (k.first == roomId) {
            if (k.second->getPin() == pin){
                // check if nick is unique
                if (!k.second->isUserAlreadyInRoom(nick)) {
                    client->setNick(nick);
                    client->setParticipatingIn(k.second);
                    k.second->addPlayer(client);
                    client->writeMessage(JOIN_ROOM,"success|");
                    // broadcast message to other users
                    k.second->sendPlayersInRoom(nullptr);
                } else {
                    client->writeMessage(JOIN_ROOM,"nick is not unique|");
                }
            }
            else {
                client->writeMessage(JOIN_ROOM,"incorrect pin|");
            }
        }
    }
}

void Server::deleteKahoot(std::pair<const int,Kahoot*> kahoot) {
    for(auto it = this->kahoots.begin(); it != this->kahoots.end(); ) {
        if(*it == kahoot)
            it = this->kahoots.erase(it);
        else
            ++it;
    }
    delete kahoot.second;
}

void Server::deleteClient(Client *client) {
    for(auto it = this->clients.begin(); it != this->clients.end(); ) {
        if(*it == client)
            it = this->clients.erase(it);
        else
            ++it;
    }
    delete client;
}
