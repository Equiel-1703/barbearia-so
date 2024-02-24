#pragma once

#include <pthread.h>
#include <unistd.h>
#include <iostream>
#include <atomic>

#include "LinkedList.h"
#include "GerenciadorFerramentas.h"
#include "Ferramentas.h"
#include "Cliente.h"

// Forward declaration of Cliente
class Cliente;

class Barbeiro
{
private:
    pthread_t tid;

    static pthread_mutex_t mutex_fila_clientes;
    static pthread_cond_t cond_tem_cliente;

    GerenciadorFerramentas *gerenciador_ferramentas;
    LinkedList<Cliente *> *clientes_antender;

    // Variável booleana atomica
    static std::atomic<bool> terminate_barbeiros_threads;

public:
    Barbeiro(LinkedList<Cliente *> *clientes_antender, GerenciadorFerramentas *gerenciador_ferramentas);
    ~Barbeiro();

    // Get mutex_fila_clientes
    static pthread_mutex_t *getFilaClientesMutex();

    // Get cond_tem_cliente
    static pthread_cond_t *getCondTemCliente();

    // Get tid
    pthread_t getTid() const;
    
    // Terminate barbeiros threads
    static void terminateBarbeirosThreads();

    // Run function
    static void *run(void *arg);
};
