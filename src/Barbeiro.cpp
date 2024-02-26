#include "Barbeiro.h"

// Declarando mutexes e conds
pthread_mutex_t Barbeiro::mutex_fila_clientes = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t Barbeiro::mutex_terminar_barbeiros = PTHREAD_MUTEX_INITIALIZER;

pthread_cond_t Barbeiro::cond_tem_cliente = PTHREAD_COND_INITIALIZER;
pthread_cond_t Barbeiro::cond_terminar_barbeiros = PTHREAD_COND_INITIALIZER;

std::atomic<int> Barbeiro::barbeiros_finalizados(0);

Barbeiro::Barbeiro(LinkedList<Cliente *> *clientes_antender, GerenciadorFerramentas *gerenciador_ferramentas, ConsoleWriter *cw, int N_BARBEIROS)
    : N_BARBEIROS(N_BARBEIROS)
{
    this->clientes_antender = clientes_antender;

    this->gerenciador_ferramentas = gerenciador_ferramentas;
    
    this->console_writer = cw;

    // Iniciar thread
    pthread_create(&this->tid, NULL, run, (void *)this);
}

Barbeiro::~Barbeiro()
{
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

// Get mutex_terminar_barbeiros
pthread_mutex_t *Barbeiro::getTerminarBarbeirosMutex()
{
    return &Barbeiro::mutex_terminar_barbeiros;
}

// Get cond_tem_cliente
pthread_cond_t *Barbeiro::getCondTemCliente()
{
    return &Barbeiro::cond_tem_cliente;
}

// Get cond_terminar_barbeiros
pthread_cond_t *Barbeiro::getCondTerminarBarbeiros()
{
    return &Barbeiro::cond_terminar_barbeiros;
}

void *Barbeiro::run(void *arg)
{
    // Pegar objeto do barbeiro dessa thread
    Barbeiro *b = (Barbeiro *)arg;
    Cliente *c;

    // Mantém controle de quantos barbeiros terminaram
    b->barbeiros_finalizados.store(b->N_BARBEIROS);

    while (true)
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

        // Pegou cliente pra atender
        b->barbeiros_finalizados--;

        // Pegar tesoura e pente
        Ferramentas *f = b->gerenciador_ferramentas->getFerramenta();

        // Liberar semáforo do cliente
        sem_post(c->getSemCliente());

        f->usarPente();
        f->usarTesoura();

        // atendendo cliente
        b->message = "Barbeiro " + std::to_string(b->getTid()) + " atendendo cliente " + std::to_string(c->getTid()) + "\n";
        b->console_writer->writeToConsole(b->message);
        pthread_join(c->getTid(), NULL);

        // Liberar tesoura e pente
        b->gerenciador_ferramentas->releaseFerramenta(&f);

        b->message = "Barbeiro " + std::to_string(b->getTid()) + " terminou de atender cliente " + std::to_string(c->getTid()) + "\n";
        b->console_writer->writeToConsole(b->message);

        b->barbeiros_finalizados++;

        b->message = std::to_string(b->barbeiros_finalizados.load()) + "\n";
        b->console_writer->writeToConsole(b->message);

        // Deleta objeto do cliente
        delete c;

        if (b->barbeiros_finalizados.load() == b->N_BARBEIROS)
        {
            b->console_writer->writeToConsole("Todos barbeiros terminaram\n");
            pthread_mutex_lock(b->getTerminarBarbeirosMutex());
            pthread_cond_signal(b->getCondTerminarBarbeiros());
            pthread_mutex_unlock(b->getTerminarBarbeirosMutex());
        }
    }
}