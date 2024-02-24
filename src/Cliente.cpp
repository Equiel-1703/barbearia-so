#include "Cliente.h"

Cliente::Cliente()
{
}

Cliente::~Cliente()
{
    // Destroy thread
    pthread_cancel(NULL);
}

// Get tid
pthread_t Cliente::getTid() const
{
    return this->tid;
}

// Iniciar thread
void Cliente::iniciar(Barbeiro *b)
{
    // Create thread
    pthread_create(&this->tid, NULL, run, (void *)b);
}

void* Cliente::run(void *arg)
{
    while (true)
    {
        // Pegar mutex do barbeiro
        pthread_mutex_t *mutex = ((Barbeiro *)arg)->getLiberaClienteMutex();
        // Só vai liberar quando o barbeiro permitir
        pthread_mutex_lock(mutex);

        std::cout << "Cliente cortando o cabelo..." << std::endl;
        sleep(2);
        std::cout << "Cliente terminou de cortar o cabelo." << std::endl;

        // Liberar mutex do barbeiro
        pthread_mutex_unlock(mutex);

        // Cliente vai embora
        pthread_exit(NULL);
    }
}
