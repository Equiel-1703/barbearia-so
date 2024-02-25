#include "Cliente.h"
#include <cstdlib>
#include <ctime>

Cliente::Cliente()
{
    // Inicializar semáforo
    sem_init(&this->sem_cliente, 0, 1);

    // Esperar barbeiro liberar
    sem_wait(&this->sem_cliente);

    // Inicializa thread
    pthread_create(&this->tid, NULL, run, (void *)this);
}

Cliente::~Cliente()
{
    // Destroy semáforo
    sem_destroy(&this->sem_cliente);
}

// Get tid
pthread_t Cliente::getTid() const
{
    return this->tid;
}

// Get sem_cliente
sem_t *Cliente::getSemCliente()
{
    return &this->sem_cliente;
}

void *Cliente::run(void *arg)
{
    Cliente *c = (Cliente *)arg;

    // Espera semáforo ser liberado pelo barbeiro
    sem_wait(c->getSemCliente());
    std::cout << "Cliente " << c->getTid() << " cortando cabelo" << std::endl;

    // Gera um número aleatório entre 3 e 6 segundos
    int tempoCorte = (rand() % 4) + 3;

    // Dorme por um tempo aleatório
    sleep(tempoCorte);

    return NULL;
}
