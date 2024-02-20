#include <iostream>
#include <pthread.h>

void *print_msg(void *ptr)
{
    std::cout << (char *) ptr << std::endl;
}

int main(int argc, char *argv[])
{
    pthread_t t1;

    char *msg = "ola mundo";
    int ret1;


    ret1 = pthread_create(&t1, NULL, print_msg, msg);

    pthread_join(t1, NULL);

    std::cout << ret1 << std::endl;

    return 0;
}
