#include "Cliente.h"

Cliente::Cliente(int tempo_corte, ConsoleWriter *cw): tempo_corte(tempo_corte)
{
    // Inicializar semáforo
    sem_init(&this->sem_cliente, 0, 1);

    // Esperar barbeiro liberar
    sem_wait(&this->sem_cliente);

    // Inicializar console_writer
    this->console_writer = cw;

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
    
    c->message = "Cliente " + std::to_string(c->getTid()) + " cortará o cabelo por " + std::to_string(c->tempo_corte) + " segundos\n";
    c->console_writer->writeToConsole(c->message);

    // Dorme por um tempo aleatório
    sleep(c->tempo_corte);

    return NULL;
}
