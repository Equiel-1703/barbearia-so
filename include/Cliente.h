#pragma once

#include <pthread.h>
#include <unistd.h>
#include <iostream>
#include <semaphore.h>

#include "Barbeiro.h"

// Forward declaration of Barbeiro
class Barbeiro;

class Cliente
{
private:
    pthread_t tid;
    int tempo_de_corte;
    Barbeiro *barbeiro;

    // Semáforo do cliente (o barbeiro que libera)
    sem_t sem_cliente;

    // Run function
    static void *run(void *arg);
public:
    Cliente();
    ~Cliente();

    // Get tid
    pthread_t getTid() const;

    // Get sem_cliente
    sem_t *getSemCliente();
};