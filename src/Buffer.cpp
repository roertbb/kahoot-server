//
// Created by roertbb on 12.01.19.
//

#include <cstdlib>
#include <cstring>
#include "Buffer.h"

Buffer::Buffer() {
    this->data = (char*) malloc(len);
}

Buffer::Buffer(const char *srcData, int srcLen) {
    this->len = srcLen;
    this->data = (char*) malloc(srcLen);
    memcpy(data,srcData,srcLen);
}

Buffer::~Buffer() {
    free(this->data);
}

void Buffer::resize() {
    this->len*=2;
    this->data = (char*) realloc(this->data, this->len);
}

int Buffer::remaining() {
    return this->len - this->pos;
}

char *Buffer::dataPos() {
    return data+pos;
}

void Buffer::writeLater(char *remainingData, int dataLen) {
    this->len = dataLen;
    this->data = (char*) malloc(dataLen);
    memcpy(data,remainingData,dataLen);
}

