#include "Cliente.h"

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

    // Destroy thread
    pthread_cancel(this->tid);
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

    // Dorme por um tempo
    sleep(2);

    return NULL;
}
