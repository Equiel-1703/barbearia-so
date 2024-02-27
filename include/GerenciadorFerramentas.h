#pragma once

#include <iostream>
#include <unistd.h>
#include <semaphore.h>

#include "Ferramentas.h"
#include "LinkedList.h"
#include "ConsoleWriter.h"

class GerenciadorFerramentas
{
private:
    sem_t semaforo;
    const int numFerramentas;
    LinkedList<Ferramentas *> ferramentas;

    ConsoleWriter *console_writer;
public:
    GerenciadorFerramentas(const int numFerramentas, ConsoleWriter *console_writer);
    ~GerenciadorFerramentas();

    // Get ferramenta
    Ferramentas *getFerramenta();

    // Release ferramenta
    void releaseFerramenta(Ferramentas **ferramenta);
};
