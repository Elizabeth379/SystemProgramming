#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <time.h>

#define NUM_BLOCKS 5
#define BLOCK_SIZE 2

typedef struct {
    int intValue;
} DataBlock;

sem_t semaphores[NUM_BLOCKS];
int sums[NUM_BLOCKS];

void *thread_function(void *args) {
    int *arg = (int *)args;
    int id = *arg;
    printf("*****\nThread ID %ld", pthread_self());
    srand(time(NULL) + id);
    for (int j = 0; j < BLOCK_SIZE; ++j) {
        sem_wait(&semaphores[id]);
        int value = rand() % 100;
        printf("\n Memory[%d][%d].intValue = %d\n", id, j, value);
        sums[id] += value;
        sem_post(&semaphores[id]);
    }
    pthread_exit(NULL);
}

int main() {
    pthread_t threads[NUM_BLOCKS];
    int id[NUM_BLOCKS];

    for (int i = 0; i < NUM_BLOCKS; ++i) {
        sem_init(&semaphores[i], 0, 1);
        id[i] = i;
        sums[i] = 0;
    }

    for (int i = 0; i < NUM_BLOCKS; ++i) {
        pthread_create(&threads[i], NULL, thread_function, &id[i]);
    }

    for (int i = 0; i < NUM_BLOCKS; ++i) {
        pthread_join(threads[i], NULL);
    }

    int total_sum = 0;
    for (int i = 0; i < NUM_BLOCKS; ++i) {
        total_sum += sums[i];
        sem_destroy(&semaphores[i]);
    }

    printf("*****\nSUM: %d\n", total_sum);

    return 0;
}
