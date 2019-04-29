#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <string>
#include <iostream>
#include <semaphore.h>

#define LINE_LEN 512

int main(int argc, char* argv[])
{
    if (argc < 4) {
        printf("fff");
        return -1;
    }
    int fd[2];
    ssize_t size;
 
    fd[0] = atoi(argv[0]);
    fd[1] = atoi(argv[1]);
    char buffer[LINE_LEN]; 

    sem_t* semaphoreRead = sem_open(argv[2], 0);
    sem_t* semaphoreWrite = sem_open(argv[3], 0);
    if (semaphoreRead == SEM_FAILED) { 
        printf("Semaphore Failed"); 
        return -1; 
    } 
    if (semaphoreWrite == SEM_FAILED) { 
        printf("Semaphore Failed"); 
        return -1; 
    }

    while(true) {
        close(fd[1]);

        sem_wait(semaphoreWrite);

        read(fd[0], buffer, 100);
        close(fd[0]);
        std::cout << "Accepted string from server process: " << buffer << std::endl;

        sem_post(semaphoreRead);

        if (strcmp(buffer, "q") == 0) {
            break;
        }
    }

    std::cout << "Client process finish work.";

    return 0;
}