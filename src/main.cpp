#include <iostream>
#include <unistd.h>
#include <pthread.h>

#include "Cliente.h"
#include "Barbeiro.h"
#include "LinkedList.h"
#include "GerenciadorFerramentas.h"

#define N_CLIENTES 5
#define N_BARBEIROS 2

int clientes_atendidos = 0;
int clientes_desistiram = 0;
int cadeiras_livres = 5;

int main(void)
{
    LinkedList<Cliente *> *fila_clientes = new LinkedList<Cliente *>();
    GerenciadorFerramentas *gerenciador_ferramentas = new GerenciadorFerramentas(N_BARBEIROS / 2);

    // Lista de barbeiros
    LinkedList<Barbeiro *> barbeiros;

    // Criando barbeiros
    for (int i = 0; i < N_BARBEIROS; i++)
    {
        Barbeiro *b = new Barbeiro(fila_clientes, gerenciador_ferramentas);
        barbeiros.push_back(b);
    }

    // criando clientes
    for (int i = 0; i < N_CLIENTES; i++)
    {
        Cliente *c = new Cliente();

        if (cadeiras_livres > 0)
        {
            fila_clientes->push_back(c);
            cadeiras_livres--;

            // Travar mutex da fila de clientes
            pthread_mutex_lock(Barbeiro::getFilaClientesMutex());
            
            // Sinalizar que chegou cliente
            pthread_cond_signal(Barbeiro::getCondTemCliente());

            // Destravar mutex da fila de clientes
            pthread_mutex_unlock(Barbeiro::getFilaClientesMutex());

            std::cout << "Cliente " << c->getTid() <<" chegou e sentou na cadeira " << i + 1 << std::endl;

        }
        else
            delete c;
    }

    // Esperar threads dos clientes terminarem
    while (!fila_clientes->isEmpty())
    {
        Cliente *c = fila_clientes->front();
        pthread_join(c->getTid(), NULL);
    }

    // Esperar threads dos barbeiros terminarem
    Barbeiro::terminateBarbeirosThreads();

    // Deletar objetos dos barbeiros
    while (!barbeiros.isEmpty())
    {
        Barbeiro *b = barbeiros.pop();
        // pthread_join(b->getTid(), NULL);
        delete b;
    }

    delete gerenciador_ferramentas;

    return 0;
}
