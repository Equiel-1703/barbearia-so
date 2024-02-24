#pragma once

#include <pthread.h>
#include <unistd.h>
#include <iostream>

#include "Barbeiro.h"

class Cliente
{
private:
    pthread_t tid;
    
    // Run function
    static void *run(void *arg);
public:
    Cliente();
    ~Cliente();

    // Get tid
    pthread_t getTid() const;

    // Iniciar thread
    void iniciar(Barbeiro *b);
};