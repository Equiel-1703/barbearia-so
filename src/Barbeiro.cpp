#include "Barbeiro.h"

Barbeiro::Barbeiro()
{
    pthread_mutex_init(&mutex_chegou_cliente, NULL);
    pthread_mutex_init(&mutex_libera_cliente, NULL);

    // Lock
    pthread_mutex_lock(&mutex_chegou_cliente);
    pthread_mutex_lock(&mutex_libera_cliente);

    // Create thread
    pthread_create(&this->tid, NULL, run, (void *)this);
}

Barbeiro::Barbeiro(Barbeiro &b)
{
    this->tid = b.getTid();
    this->tid_cliente = b.getTidCliente();
    this->mutex_libera_cliente = *b.getLiberaClienteMutex();
    this->mutex_chegou_cliente = *b.getChegouClienteMutex();
}

Barbeiro::~Barbeiro()
{
    // Destroy thread
    pthread_cancel(this->getTid());

    // Destroy mutexes
    pthread_mutex_destroy(&mutex_libera_cliente);
    pthread_mutex_destroy(&mutex_chegou_cliente);
}

pthread_t Barbeiro::getTid() const
{
    return this->tid;
}

pthread_mutex_t *Barbeiro::getLiberaClienteMutex() const
{
    return (pthread_mutex_t *)&this->mutex_libera_cliente;
}

pthread_mutex_t *Barbeiro::getChegouClienteMutex() const
{
    return (pthread_mutex_t *)&this->mutex_chegou_cliente;
}

pthread_t Barbeiro::getTidCliente() const
{
    return this->tid_cliente;
}

void Barbeiro::setTidCliente(pthread_t tid)
{
    this->tid_cliente = tid;
}

void *Barbeiro::run(void *arg)
{
    // Pegar objeto do barbeiro
    Barbeiro *b = (Barbeiro *)arg;

    while (true)
    {
        // Ver se chegou cliente
        pthread_mutex_lock(b->getChegouClienteMutex());
        std::cout << "Barbeiro " << b->getTid() << " foi avisado que chegou cliente." << std::endl;

        // Pegar tesoura e pente
        // ....
        sleep(2);
        std::cout << "Barbeiro " << b->getTid() << " pegou tesoura e pente." << std::endl;

        // Liberar mutex para atender o cliente
        pthread_mutex_unlock(b->getLiberaClienteMutex());

        // atendendo cliente
        pthread_join(b->getTidCliente(), NULL);

        // Liberar tesoura e pente
        // ...
        std::cout << "Barbeiro " << b->getTid() << " está liberando tesoura e pente." << std::endl;

        // Barbeiro está livre
        pthread_mutex_lock(b->getChegouClienteMutex());
        pthread_mutex_lock(b->getLiberaClienteMutex());

        std::cout << "Barbeiro " << b->getTid() << " terminou atendimento." << std::endl;
    }
}
