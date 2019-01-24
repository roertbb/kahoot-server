//
// Created by roertbb on 12.01.19.
//

#ifndef KAHOOT_SERVER_BUFFER_H
#define KAHOOT_SERVER_BUFFER_H


class Buffer {
public:
    char * data;
    int len = 64;
    int pos = 0;
    bool pending = false;

    Buffer();
    Buffer(const char* srcData, int srcLen);
    ~Buffer();
    void resize();
    int remaining();
    char * dataPos();
    void writeLater(char * data, int len);
    void appendToBuffer(char * data, int len);
};


#endif //KAHOOT_SERVER_BUFFER_H
