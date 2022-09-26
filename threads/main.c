#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>

volatile int next_id = 0;
static sem_t id_lock;
static sem_t connections_lock;
static sem_t operators_lock;
static int NUM_OPERATORS = 4;
static int NUM_LINES = 7;
static int connections = 0;

void *phonecall(void *vargp);

int main(int argc, char **argv)
{
    //check for valid program start up
    if (argc != 2)
    {
        printf("usage: %s <num_phonecalls>\n", argv[0]);
        return 1;
    }

    // check for valid input
    for (int i = 0; argv[1][i] != '\0'; ++i) {
        if (argv[1][i] < '0' || argv[1][i] > '9') {
            printf("invalid input\n");
            return 2;
        }
    }

    int NUM_PHONECALLS = atoi(argv[1]);
    pthread_t tids[NUM_PHONECALLS]; // array of thread ids

    // initialize all semaphores
    sem_init(&id_lock, 0, 1);
    sem_init(&connections_lock, 0, 1);
    sem_init(&operators_lock, 0, NUM_OPERATORS); 

    // create threads
    for (int i = 0; i < NUM_PHONECALLS; ++i)
    {
        pthread_create(tids + i, NULL, phonecall, NULL);
    }

    // join threads
    for (int i = 0; i < NUM_PHONECALLS; ++i)
    {
        pthread_join(tids[i], NULL);
    }

    // destroy all semaphores
    sem_destroy(&id_lock);
    sem_destroy(&connections_lock);
    sem_destroy(&operators_lock);

    return 0;
}

void *phonecall(void *vargp)
{
    // critical zone for dealing with global variable next_id
    sem_wait(&id_lock);
    int i = ++next_id;
    sem_post(&id_lock);

    printf("%d id: connection attempt...\n", i);
    // critical zone for checking for connection and creating connection
    sem_wait(&connections_lock);
    if (connections == NUM_LINES)
    {
        printf("%d id: connection failure\n", i);
    }
    else
    {
        ++connections;
        printf("%d id: connection success\n", i);
    }
    sem_post(&connections_lock);
    // critical zone for connecting phone calls to operators. Uses counting semaphore
    sem_wait(&operators_lock);
    printf("%d id: processing order...\n", i);
    sleep(3);
    printf("%d id: order processed!\n", i);
    // ciritical zone for terminating a connection (decrement number of connections)
    sem_wait(&connections_lock);
    --connections;
    sem_post(&connections_lock);
    printf("%d id: connection terminated.\n", i);
    sem_post(&operators_lock);
}
