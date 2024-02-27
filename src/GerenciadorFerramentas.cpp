#include "GerenciadorFerramentas.h"

GerenciadorFerramentas::GerenciadorFerramentas(const int numFerramentas, ConsoleWriter *console_writer)
    : numFerramentas(numFerramentas), console_writer(console_writer)
{
    // Inicializa o semáforo
    sem_init(&semaforo, 0, numFerramentas);

    // Adiciona ferramentas na LinkedList
    for (int i = 0; i < numFerramentas; i++)
    {
        ferramentas.push_back(new Ferramentas(console_writer));
    }
}

GerenciadorFerramentas::~GerenciadorFerramentas()
{
    // Destrói o semáforo
    sem_destroy(&semaforo);

    // Deleta ferramentas na LinkedList
    for (int i = 0; i < numFerramentas; i++)
    {
        delete ferramentas.pop("del ferramenta");
    }
}

Ferramentas *GerenciadorFerramentas::getFerramenta()
{
    // Aguarda a disponibilidade de uma ferramenta
    sem_wait(&semaforo);

    // Retorna a ferramenta
    return ferramentas.pop("retorna ferramenta");
}

void GerenciadorFerramentas::releaseFerramenta(Ferramentas **ferramenta)
{
    // Adiciona a ferramenta de volta na LinkedList
    ferramentas.push_back(*ferramenta);

    // Reseta o ponteiro
    *ferramenta = nullptr;

    // Libera a ferramenta
    sem_post(&semaforo);
}
