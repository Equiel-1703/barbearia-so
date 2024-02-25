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
    static pthread_mutex_t mutex_terminar_barbeiros;
    static pthread_cond_t cond_tem_cliente;
    static pthread_cond_t cond_terminar_barbeiros;

    GerenciadorFerramentas *gerenciador_ferramentas;
    LinkedList<Cliente *> *clientes_antender;

    // Contador de barbeiros que terminaram atendimento
    static std::atomic<int> barbeiros_finalizados;

public:
    const int N_BARBEIROS;

    Barbeiro(LinkedList<Cliente *> *clientes_antender, GerenciadorFerramentas *gerenciador_ferramentas, int N_BARBEIROS);
    ~Barbeiro();

    // Get mutex_fila_clientes
    static pthread_mutex_t *getFilaClientesMutex();

    // Get mutex_terminar_barbeiros
    static pthread_mutex_t *getTerminarBarbeirosMutex();

    // Get cond_tem_cliente
    static pthread_cond_t *getCondTemCliente();

    // Get cond_terminar_barbeiros
    static pthread_cond_t *getCondTerminarBarbeiros();

    // Get tid
    pthread_t getTid() const;

    // Run function
    static void *run(void *arg);
};
