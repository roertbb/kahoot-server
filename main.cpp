#include <iostream>
#include "src/Server.h"

int main() {
    Server * server = new Server();
    server->run();

    return 0;
}