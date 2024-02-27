#include "Ferramentas.h"

// Constructor
Ferramentas::Ferramentas(ConsoleWriter *console_writer)
{
    this->console_writer = console_writer;
}

void Ferramentas::usarPente()
{
    // Simula o uso do pente
    console_writer->writeToConsole("Usando pente...\n");
    sleep(1);
}

void Ferramentas::usarTesoura()
{
    // Simula o uso da tesoura
    console_writer->writeToConsole("Usando tesoura...\n");
    sleep(1);
}

