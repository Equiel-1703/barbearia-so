#pragma once

#include <pthread.h>
#include <iostream>

class ConsoleWriter
{
private:
    pthread_mutex_t mutex;
public:
    ConsoleWriter();
    ~ConsoleWriter();

    void writeToConsole(std::string &message);
    void writeToConsole(const char *message);
};

