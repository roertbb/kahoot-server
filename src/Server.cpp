//
// Created by roertbb on 22.12.18.
//
#include <cerrno>
#include <error.h>
#include "Server.h"

Server::Server() {
    this->initSocketConnection();
    this->handlePool();
}

int Server::initSocketConnection() {

    this->server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (this->server_fd < 0) {
        perror("Creating socket failed\n");
        return 1;
    }

    int n = 1;
    setsockopt(this->server_fd, SOL_SOCKET, SO_REUSEADDR, &n, sizeof(n));

    // set non-blocking modeint handlePool();
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

int Server::handlePool() {
    int epoll_fd = epoll_create1(0);

    epoll_event ee {EPOLLIN, {.fd = this->server_fd}};
    epoll_ctl(epoll_fd, EPOLL_CTL_ADD, this->server_fd, &ee);

    while(true) {
        if ((epoll_wait(epoll_fd, &ee, 1, -1)) == -1) {
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

            printf("new connection from: %s:%hu (fd: %d)\n", inet_ntoa(client_address.sin_addr), ntohs(client_address.sin_port), client_fd);

            //TODO: insert client to set
            ClientHandler * c = new ClientHandler(client_fd, epoll_fd);
        } else {
        // handle events from users
            ((Handler*)ee.data.ptr)->handleEvent(ee.events);
        }
    }
}


