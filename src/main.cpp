#include <iostream>
#include <unistd.h>
#include <pthread.h>

#include "Cliente.h"
#include "Barbeiro.h"
#include "LinkedList.h"

#define N_CLIENTES 1
#define N_BARBEIROS 1

int clientes_atendidos = 0;
int clientes_desistiram = 0;
int cadeiras_livres = N_BARBEIROS;

int main(void)
{
    LinkedList<Cliente *> clientes;
    LinkedList<Barbeiro *> barbeiros;

    // Criando barbeiros
    for (int i = 0; i < N_BARBEIROS; i++)
    {
        Barbeiro *b = new Barbeiro();
        barbeiros.push_back(b);
    }

    // criando clientes
    for (int i = 0; i < N_CLIENTES; i++)
    {
        Cliente *c = new Cliente();

        if (cadeiras_livres > 0)
        {
            std::cout << "Cliente chegou e sentou na cadeira " << i << std::endl;

            clientes.push_back(c);
            cadeiras_livres--;

            std::cout << "Cadeiras livres: " << cadeiras_livres << std::endl;

            // Pega o barbeiro da frente da lista
            Barbeiro *b = barbeiros.pop();

            // Coloca o cliente para o barbeiro atender e inicia thread do cliente
            b->setTidCliente(c->getTid());
            c->iniciar(b);

            std::cout << "Barbeiro " << b->getTid() << " atendendo cliente " << c->getTid() << std::endl;
            // Libera o barbeiro para atender o cliente
            pthread_mutex_unlock(b->getChegouClienteMutex());

            // Remove o barbeiro da frente da lista e coloca no fim
            barbeiros.push_back(b);
            std::cout << "Barbeiro foi pro fim da lista" << std::endl;
        }
        else
            delete c;
    }

    // Esperar threads dos clientes terminarem
    for (int i = 0; i < N_CLIENTES; i++)
    {
        Cliente *c = clientes.pop();
        pthread_join(c->getTid(), NULL);
        delete c;
    }

    // Encerrar threads dos barbeiros
    for (int i = 0; i < N_BARBEIROS; i++)
    {
        Barbeiro *b = barbeiros.pop();
        delete b;
    }

    return 0;
}
