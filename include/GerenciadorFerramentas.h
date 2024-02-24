#pragma once

#include <iostream>
#include <unistd.h>
#include <semaphore.h>

#include "Ferramentas.h"
#include "LinkedList.h"

class GerenciadorFerramentas
{
private:
    sem_t semaforo;
    LinkedList<Ferramentas *> ferramentas;
    const int numFerramentas;
public:
    GerenciadorFerramentas(const int numFerramentas);
    ~GerenciadorFerramentas();

    // Get ferramenta
    Ferramentas *getFerramenta();

    // Release ferramenta
    void releaseFerramenta(Ferramentas **ferramenta);
};
