//
// Created by roertbb on 22.12.18.
//
#include "Server.h"

Server::Server() {
    this->initSocketConnection();
}

int Server::initSocketConnection() {

    this->socket_desc = socket(AF_INET, SOCK_STREAM, 0);
    if (this->socket_desc < 0) {
        perror("Creating socket failed");
        return 1;
    }

    int n = 1;
    setsockopt(this->socket_desc, SOL_SOCKET, SO_REUSEADDR, &n, sizeof(n));

    // set non-blocking mode
    fcntl(this->socket_desc, F_SETFL, O_NONBLOCK);

    // TODO: create config file with server data
    sockaddr_in server_data {
        .sin_family = AF_INET,
        .sin_port = htons(1234),
        .sin_addr = {inet_addr("127.0.0.1")}
    };

    if ((bind(this->socket_desc, (sockaddr *) &server_data, sizeof(server_data))) < 0) {
        perror("Binding socket failed");
        return 1;
    }

    if ((listen(this->socket_desc, 10)) < 0) {
        perror("Creating queue for listening sockets failed");
        return 1;
    }

    printf("Listening for client's connections\n");
}
