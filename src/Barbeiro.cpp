#include "Barbeiro.h"

// Declarando variavel atomica
std::atomic<bool> Barbeiro::terminate_barbeiros_threads(false);

// Declarando mutex e cond
pthread_mutex_t Barbeiro::mutex_fila_clientes = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t Barbeiro::cond_tem_cliente = PTHREAD_COND_INITIALIZER;

Barbeiro::Barbeiro(LinkedList<Cliente *> *clientes_antender, GerenciadorFerramentas *gerenciador_ferramentas)
{
    this->clientes_antender = clientes_antender;

    this->gerenciador_ferramentas = gerenciador_ferramentas;

    // Iniciar thread
    pthread_create(&this->tid, NULL, run, (void *)this);
}

Barbeiro::~Barbeiro()
{
    // Destroy thread
    pthread_cancel(this->getTid());

    // Destroy mutex da fila
    pthread_mutex_destroy(&this->mutex_fila_clientes);
}

pthread_t Barbeiro::getTid() const
{
    return this->tid;
}

// Get mutex_fila_clientes
pthread_mutex_t *Barbeiro::getFilaClientesMutex()
{
    return &Barbeiro::mutex_fila_clientes;
}

// Get cond_tem_cliente
pthread_cond_t *Barbeiro::getCondTemCliente()
{
    return &Barbeiro::cond_tem_cliente;
}

void Barbeiro::terminateBarbeirosThreads()
{
    terminate_barbeiros_threads.store(true);
}

void *Barbeiro::run(void *arg)
{
    // Pegar objeto do barbeiro dessa threads
    Barbeiro *b = (Barbeiro *)arg;
    Cliente *c;

    while (!terminate_barbeiros_threads.load())
    {
        // Verificar fila de clientes
        pthread_mutex_lock(b->getFilaClientesMutex());
        if (b->clientes_antender->isEmpty())
        {
            pthread_cond_wait(&b->cond_tem_cliente, b->getFilaClientesMutex());

            // Depois do sinal, pegar elemento da fila
            c = b->clientes_antender->pop();
        }
        else
        {
            c = b->clientes_antender->pop();
        }
        pthread_mutex_unlock(b->getFilaClientesMutex());

        

        // Pegar tesoura e pente
        Ferramentas *f = b->gerenciador_ferramentas->getFerramenta();

        // Liberar semáforo do cliente
        sem_post(c->getSemCliente());

        f->usarPente();
        f->usarTesoura();

        // atendendo cliente
        // std::cout << "Esperando thread " << c->getTid() << " terminar" << std::endl;
        pthread_join(c->getTid(), NULL);

        // Liberar tesoura e pente
        b->gerenciador_ferramentas->releaseFerramenta(&f);

        std::cout << "Barbeiro " << b->getTid() << " atendeu cliente " << c->getTid() << std::endl;
    }

    // Saindo da thread
    pthread_exit(NULL);
}
