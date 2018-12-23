//
// Created by roertbb on 23.12.18.
//

#ifndef KAHOOT_SERVER_HANDLER_H
#define KAHOOT_SERVER_HANDLER_H

#include <cstdint>

class Handler {
public:
    virtual ~ Handler(){};
    virtual void handleEvent(uint32_t events) = 0;
};

#endif //KAHOOT_SERVER_HANDLER_H
