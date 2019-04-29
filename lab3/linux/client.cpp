#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <string>
#include <cstring>
#include <iostream>
#include <semaphore.h>

#define LINE_LEN 512

int main(int argc, char* argv[])
{
    if (argc < 4) {
        printf("Faild to init client\n");
        return -1;
    }
    int fd[2];
    ssize_t size;
 
    fd[0] = atoi(argv[0]);
    fd[1] = atoi(argv[1]);
    
    sem_t* semaphoreRead = sem_open(argv[2], 0);
    if (semaphoreRead == SEM_FAILED) { 
        printf("Semaphore Failed\n"); 
        sem_close(semaphoreRead);
        return -1; 
    } 
    sem_t* semaphoreWrite = sem_open(argv[3], 0);
    if (semaphoreWrite == SEM_FAILED) { 
        printf("Semaphore Failed\n"); 
        return -1; 
    }

    close(fd[1]); 
    while(true) {
        char buffer[LINE_LEN]; 
        
        sem_wait(semaphoreWrite);
        read(fd[0], buffer, LINE_LEN);
        sem_post(semaphoreRead);
        
        if (strcmp(buffer, "q") == 0) {
            printf("aa waited\n");
            break;
        }
    }
    close(fd[0]);

    sem_close(semaphoreWrite);
    sem_close(semaphoreRead);

    return 0;
}