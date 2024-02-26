#include "ConsoleWriter.h"

ConsoleWriter::ConsoleWriter()
{
    pthread_mutex_init(&mutex, NULL);
}

ConsoleWriter::~ConsoleWriter()
{
    pthread_mutex_destroy(&mutex);
}

void ConsoleWriter::writeToConsole(std::string &message)
{
    pthread_mutex_lock(&mutex);
    std::cout << message;
    fflush(stdout);
    pthread_mutex_unlock(&mutex);
}

void ConsoleWriter::writeToConsole(const char *message)
{
    pthread_mutex_lock(&mutex);
    std::cout << message;
    fflush(stdout);
    pthread_mutex_unlock(&mutex);
}