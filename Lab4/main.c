#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <semaphore.h>
#include <time.h>

#define NUM_BLOCKS 5 
#define BLOCK_SIZE 1

typedef struct {
    int intValue;
} DataBlock;

void create_agents(key_t key, int id[], DataBlock *memory[], sem_t *sem) {
    int i;
    for (i = 0; i < NUM_BLOCKS; ++i) {
        pid_t pid = fork();
        if (pid == -1) {
            perror("fork");
            exit(1);
        } else if (pid == 0) {
            key_t process_key = key + i; 
            sem_wait(sem); 
            printf("*****\nProcess %d\n", getpid());
            srand(time(NULL) + getpid() + i);
            for (int j = 0; j < BLOCK_SIZE; ++j) {
                memory[i][j].intValue = rand() % 100;
                printf("\n Shared memory ID: %d\n Key: %d\n Memory[%d][%d].intValue = %d\n",
                       id[i], process_key, i, j, memory[i][j].intValue);
            }
            printf("*****");
            sem_post(sem); 
            exit(0);
        }
    }
}

int main() {
    int id[NUM_BLOCKS];
    key_t key;
    DataBlock *memory[NUM_BLOCKS];
    
    sem_t sem;
    
    int i;

    if (sem_init(&sem, 1, 1) == -1) {
        perror("sem_init");
        exit(1);
    }

    if ((key = ftok(".", 'R')) == -1) {
        perror("ftok");
        exit(1);
    }

    printf("*****\nMain Process PID: %d\n", getpid());

    for (i = 0; i < NUM_BLOCKS; ++i) {
        key_t process_key = key + i;
        if ((id[i] = shmget(process_key, BLOCK_SIZE * sizeof(DataBlock), IPC_CREAT | IPC_EXCL | 0644)) == -1) {
            perror("shmget");
            exit(1);
        }
        printf("Shared memory for Block %d: ID: %d, Key: %d\n", i + 1, id[i], process_key);
        memory[i] = (DataBlock *)shmat(id[i], NULL, 0);
        if (memory[i] == (DataBlock *) -1) {
            perror("shmat");
            exit(1);
        }
    }

    for (i = 0; i < NUM_BLOCKS; ++i) {
        for (int j = 0; j < BLOCK_SIZE; ++j) {
            memory[i][j].intValue = 0;
        }
    }

    create_agents(key, id, memory, &sem);

    for (i = 0; i < NUM_BLOCKS; ++i) {
        wait(NULL);
    }

    int sum = 0;
    for (i = 0; i < NUM_BLOCKS; ++i) {
        for (int j = 0; j < BLOCK_SIZE; ++j) {
            sum += memory[i][j].intValue;
        }
    }

    printf("\nSUM: %d\n", sum);

    for (i = 0; i < NUM_BLOCKS; ++i) {
        if (shmdt(memory[i]) == -1) {
            perror("shmdt");
            exit(1);
        }
    }

    for (i = 0; i < NUM_BLOCKS; ++i) {
        if (shmctl(id[i], IPC_RMID, NULL) == -1) {
            perror("shmctl");
            exit(1);
        }
    }

    if (sem_destroy(&sem) == -1) {
        perror("sem_destroy");
    }

    return 0;
}

