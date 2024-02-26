#include <iostream>
#include <unistd.h>
#include <pthread.h>

#include "Cliente.h"
#include "Barbeiro.h"
#include "LinkedList.h"
#include "GerenciadorFerramentas.h"
#include "ConsoleWriter.h"

#define N_CLIENTES 7
#define N_BARBEIROS 4

int clientes_atendidos = 0;
int clientes_desistiram = 0;
int cadeiras_livres = N_CLIENTES - 2;

int main(void)
{
    LinkedList<Cliente *> *fila_clientes = new LinkedList<Cliente *>();
    GerenciadorFerramentas *gerenciador_ferramentas = new GerenciadorFerramentas(N_BARBEIROS / 2);

    // Lista de barbeiros
    LinkedList<Barbeiro *> barbeiros;

    // ConsoleWriter
    ConsoleWriter *console_writer = new ConsoleWriter();

    // Criando barbeiros
    for (int i = 0; i < N_BARBEIROS; i++)
    {
        Barbeiro *b = new Barbeiro(fila_clientes, gerenciador_ferramentas, console_writer, N_BARBEIROS);
        barbeiros.push_back(b);
    }

    // Configura semente para números aleatórios
    srand(time(NULL));

    // criando clientes
    for (int i = 0; i < N_CLIENTES; i++)
    {
        Cliente *c = new Cliente((rand() % 4) + 3, console_writer);

        if (cadeiras_livres > 0)
        {
            // Travar mutex da fila de clientes
            pthread_mutex_lock(Barbeiro::getFilaClientesMutex());

            fila_clientes->push_back(c);
            cadeiras_livres--;

            // Mais um cliente atendido
            clientes_atendidos++;

            // Destravar mutex da fila de clientes
            pthread_mutex_unlock(Barbeiro::getFilaClientesMutex());

            // Sinalizar que chegou cliente
            pthread_cond_signal(Barbeiro::getCondTemCliente());

            std::cout << "Cliente " << c->getTid() << " chegou e sentou na cadeira " << i + 1 << "\n";
        }
        else
        {
            // Cliente desistiu
            clientes_desistiram++;

            delete c;
        }
    }

    pthread_mutex_t *mutex_terminar_barbeiros = Barbeiro::getTerminarBarbeirosMutex();
    pthread_cond_t *cond_terminar_barbeiros = Barbeiro::getCondTerminarBarbeiros();

    // Esperar threads dos barbeiros terminarem
    pthread_mutex_lock(mutex_terminar_barbeiros);

    std::cout << "Esperando sinal\n";
    fflush(stdout);
    pthread_cond_wait(cond_terminar_barbeiros, mutex_terminar_barbeiros);
    
    pthread_mutex_unlock(mutex_terminar_barbeiros);

    // Deletar objetos dos barbeiros
    while (!barbeiros.isEmpty())
    {
        Barbeiro *b = barbeiros.pop();
        delete b;
    }

    delete gerenciador_ferramentas;
    delete fila_clientes;
    delete console_writer;

    // Exibir o número de clientes atendidos e desistentes
    std::cout << "Clientes atendidos: " << clientes_atendidos << std::endl;
    std::cout << "Clientes que desistiram: " << clientes_desistiram << std::endl;

    return 0;
}
