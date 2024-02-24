#include "GerenciadorFerramentas.h"

GerenciadorFerramentas::GerenciadorFerramentas(const int numFerramentas): numFerramentas(numFerramentas)
{
    // Inicializa o semáforo
    sem_init(&semaforo, 0, numFerramentas);

    // Adiciona ferramentas na LinkedList
    for (int i = 0; i < numFerramentas; i++)
    {
        ferramentas.push_back(new Ferramentas());
    }
}

GerenciadorFerramentas::~GerenciadorFerramentas()
{
    // Destrói o semáforo
    sem_destroy(&semaforo);

    // Deleta ferramentas na LinkedList
    for(int i = 0; i < numFerramentas; i++)
    {
        delete ferramentas.pop();
    }
}

Ferramentas *GerenciadorFerramentas::getFerramenta()
{
    // Aguarda a disponibilidade de uma ferramenta
    sem_wait(&semaforo);

    // Retorna a ferramenta
    return ferramentas.pop();
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
