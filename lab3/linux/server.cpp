#include <ctime>    
#include <unistd.h>
#include <sstream>
#include <string>
#include <cstring>
#include <cstdlib>
#include <iostream>
#include <semaphore.h>

#define LINE_LEN 512
#define READ_SEM "/read_sem"
#define WRITE_SEM "/write_sem"

int main() {
    int fd[2];

    if (pipe(fd) == -1) { 
        printf("Pipe Failed"); 
        return -1; 
    } 
    
    sem_t* semaphoreRead = sem_open(READ_SEM, O_CREAT, 0777, 0);
    sem_t* semaphoreWrite = sem_open(WRITE_SEM, O_CREAT, 0777, 0);
    if (semaphoreRead == SEM_FAILED) { 
        printf("Semaphore Failed Read\n"); 
        return -1; 
    } 
    if (semaphoreWrite == SEM_FAILED) { 
        printf("Semaphore Failed Write\n"); 
        return -1; 
    }

    int pid = fork(); 
    switch(pid) {
    case -1: 
    	return -1; 
    case 0: 
        execl("client.out",
        std::to_string(fd[0]).c_str(),
        std::to_string(fd[1]).c_str(),
        READ_SEM,
        WRITE_SEM,
        NULL);
    	return 0;
    default:
        char userInput[LINE_LEN];
        std::cout << "To finish server process enter q. Enter string:" << std::endl;
        while(true) {
            close(fd[0]);
            std::cin >> userInput;
            write(fd[1], userInput, strlen(userInput) + 1); 
            close(fd[1]); 

            sem_post(semaphoreWrite);
            
            std::cout << "Send string to client process: " << userInput << std::endl;
            

            if (strcmp(userInput, "q") == 0) {
                break;
            }
            sem_wait(semaphoreRead);
        }
      
        break;
    }

    sem_close(semaphoreWrite);
    sem_close(semaphoreRead);

    return 0;
}
