#include "Barbeiro.h"

// Declarando mutex e cond para controle da fila de clientes
pthread_mutex_t Barbeiro::mutex_fila_clientes = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t Barbeiro::cond_tem_cliente = PTHREAD_COND_INITIALIZER;

// Declarando mutex e cond para controle de termino dos barbeiros
pthread_mutex_t Barbeiro::mutex_terminar_barbeiros = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t Barbeiro::cond_terminar_barbeiros = PTHREAD_COND_INITIALIZER;

// Declarando flags
std::atomic<bool> Barbeiro::flag_barbeiros_sinalizaram_termino(false);

// Número de barbeiros
const int Barbeiro::N_BARBEIROS = 4;

std::atomic<int> Barbeiro::barbeiros_finalizados(0);

Barbeiro::Barbeiro(LinkedList<Cliente *> *clientes_antender, GerenciadorFerramentas *gerenciador_ferramentas, ConsoleWriter *cw)
{
    this->clientes_antender = clientes_antender;

    this->gerenciador_ferramentas = gerenciador_ferramentas;

    this->console_writer = cw;

    // Iniciar thread
    pthread_create(&this->tid, NULL, run, (void *)this);
}

Barbeiro::~Barbeiro()
{
    // Matar thread
    pthread_cancel(this->tid);
}

pthread_t Barbeiro::getTid() const
{
    return this->tid;
}

int Barbeiro::getId() const
{
    return this->tid % 10'000;
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

// Get boolean de sinalização de término dos barbeiros
bool Barbeiro::getFlagBarbeirosSinalizaramTermino()
{
    return Barbeiro::flag_barbeiros_sinalizaram_termino.load();
}

void *Barbeiro::run(void *arg)
{
    // Pegar objeto do barbeiro dessa thread
    Barbeiro *b = (Barbeiro *)arg;
    Cliente *c;

    while (true)
    {
        // Verificar fila de clientes
        pthread_mutex_lock(b->getFilaClientesMutex());
        // std::cout << "Tam lista: " << b->clientes_antender->getSize() << "\n";
        if (b->clientes_antender->isEmpty())
        {
            b->barbeiros_finalizados++;

            b->message = "Barbeiro " + std::to_string(b->getId()) + " foi de dormes - " + std::to_string(b->barbeiros_finalizados.load()) + "\n";
            b->console_writer->writeToConsole(b->message);

            if (b->barbeiros_finalizados.load() == b->N_BARBEIROS)
            {
                b->console_writer->writeToConsole("\nTodos os barbeiros dormiram\n");
                pthread_mutex_lock(b->getTerminarBarbeirosMutex());

                pthread_cond_signal(b->getCondTerminarBarbeiros());
                flag_barbeiros_sinalizaram_termino.store(true);

                pthread_mutex_unlock(b->getTerminarBarbeirosMutex());
            }

            pthread_cond_wait(&b->cond_tem_cliente, b->getFilaClientesMutex());

            // Depois do sinal, pegar elemento da fila
            c = b->clientes_antender->pop();

            // Pegou cliente pra atender
            b->barbeiros_finalizados--;

            // Sinalizar que barbeiro pegou cliente (a lista tinha ficado vazia falsamente)
            flag_barbeiros_sinalizaram_termino.store(false);
        }
        else
        {
            if (b->clientes_antender->getSize() < 0)
            {
                exit(EXIT_FAILURE);
            }

            // std::cout << "Thread: " << b->getTid() << "\n";
            c = b->clientes_antender->pop();
        }
        pthread_mutex_unlock(b->getFilaClientesMutex());

        if (c == NULL)
        {
            // std::cout << "Cliente nulo\n";
            // Outra thread pegou antes o cliente antes dessa
            continue;
        }

        // Pegar tesoura e pente
        Ferramentas *f = b->gerenciador_ferramentas->getFerramenta();

        b->message = "Barbeiro " + std::to_string(b->getId()) + " atendendo cliente " + std::to_string(c->getId()) + "\n";
        b->console_writer->writeToConsole(b->message);

        // Liberar semáforo do cliente
        sem_post(c->getSemCliente());

        f->usarPente();
        f->usarTesoura();

        // atendendo cliente
        pthread_join(c->getTid(), NULL);

        // Liberar tesoura e pente
        b->gerenciador_ferramentas->releaseFerramenta(&f);

        b->message = "Barbeiro " + std::to_string(b->getId()) + " terminou de atender cliente " + std::to_string(c->getId()) + "\n";
        b->console_writer->writeToConsole(b->message);

        // Deleta objeto do cliente
        delete c;
    }
}