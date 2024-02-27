#pragma once

#include <pthread.h>
#include <unistd.h>
#include <iostream>
#include <atomic>

#include "LinkedList.h"
#include "GerenciadorFerramentas.h"
#include "Ferramentas.h"
#include "Cliente.h"
#include "ConsoleWriter.h"

// Forward declaration of Cliente
class Cliente;

class Barbeiro
{
private:
    pthread_t tid;

    static pthread_mutex_t mutex_fila_clientes;
    static pthread_cond_t cond_tem_cliente;

    static pthread_mutex_t mutex_terminar_barbeiros;
    static pthread_cond_t cond_terminar_barbeiros;

    // Booleans que controlam o término de execução das threads dos barbeiros
    static std::atomic<bool> flag_barbeiros_sinalizaram_termino;

    GerenciadorFerramentas *gerenciador_ferramentas;
    LinkedList<Cliente *> *clientes_antender;

    ConsoleWriter *console_writer;
    std::string message;

    // Contador de barbeiros que terminaram atendimento
    static std::atomic<int> barbeiros_finalizados;

public:
    static const int N_BARBEIROS;
    static sem_t sem_terminar_barbeiros;

    Barbeiro(LinkedList<Cliente *> *clientes_antender, GerenciadorFerramentas *gerenciador_ferramentas, ConsoleWriter *cw);
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

    // Get ID
    int getId() const;

    // Get boolean de sinalização de término dos barbeiros
    static bool getFlagBarbeirosSinalizaramTermino();

    // Run function
    static void *run(void *arg);
};
