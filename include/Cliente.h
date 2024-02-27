#pragma once

#include <pthread.h>
#include <unistd.h>
#include <iostream>
#include <semaphore.h>

#include "Barbeiro.h"
#include "ConsoleWriter.h"

// Forward declaration of Barbeiro
class Barbeiro;

class Cliente
{
private:
    pthread_t tid;
    int tempo_corte;

    // Semáforo do cliente (o barbeiro que libera)
    sem_t sem_cliente;

    // ConsoleWriter
    ConsoleWriter *console_writer;
    std::string message;

    // Run function
    static void *run(void *arg);
public:
    Cliente(int tempo_corte, ConsoleWriter *cw);
    ~Cliente();

    // Get tid
    pthread_t getTid() const;

    // Get Id
    int getId() const;

    // Get sem_cliente
    sem_t *getSemCliente();
};