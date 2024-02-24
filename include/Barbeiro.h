#pragma once

#include <pthread.h>
#include <unistd.h>
#include <iostream>

class Barbeiro
{
private:
    pthread_t tid;
    pthread_t tid_cliente;

    pthread_mutex_t mutex_libera_cliente;
    pthread_mutex_t mutex_chegou_cliente;
    
public:
    Barbeiro();
    
    // Copy constructor
    Barbeiro(Barbeiro &b);

    ~Barbeiro();

    // Get mutex_libera_cliente
    pthread_mutex_t *getLiberaClienteMutex() const;

    // Get mutex chegou_cliente
    pthread_mutex_t *getChegouClienteMutex() const;

    // Get tid
    pthread_t getTid() const;

    // Get cliente tid
    pthread_t getTidCliente() const;
    // Set cliente tid
    void setTidCliente(pthread_t tid);
    
    // Run function
    static void *run(void *arg);
};
