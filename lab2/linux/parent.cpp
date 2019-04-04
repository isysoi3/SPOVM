#include <iostream>
#include <stack> 
#include <sys/types.h>
#include <signal.h>

#include <ctime>    
#include <unistd.h>
#include <ncurses.h>
#include <sstream>
#include <string>
#include <cstring>
#include <sys/wait.h>
#include <sys/mman.h>
#include <cstdlib>

#define DELAY_TIME_SEC 2

std::stack <pid_t> childPids; 

void printMenu() {
    std::cout << "[+] - add process\n";
    std::cout << "[-] - remove process\n";
    std::cout << "[q] - exit\n";
}

void addChildProcess() {
    pid_t pid = fork();
    switch(pid) {
        case -1: 
            //failed to frok
            break;
        case 0: //child
            while (true) {
                std::cout << "Process: " << getpid() << std::endl;
                sleep(DELAY_TIME_SEC);
            }
            break;	
        default: //parent
            childPids.push(pid);
            break;
    }
}

void removeChildProcess() {
    if (childPids.empty()) {
        std::cout << "Nothing to delete.\n";
    } else {
        pid_t childPid = childPids.top();
        childPids.pop();
        kill(childPid, SIGKILL);
    }
}

void removeAllChildProcess() {
    while(!childPids.empty()) {
        pid_t childPid = childPids.top();
        childPids.pop();
        kill(childPid, SIGKILL);
    }
}


int main() {
    char operation;
    bool isFinished = false;

    while(!isFinished) {
        printMenu();
        std::cin >> operation;
        switch (operation) {
            case '+':
                addChildProcess();
                break;
            case '-':
                removeChildProcess();
                break;
            case 'q':
                removeAllChildProcess();
                isFinished = true;
                break;
            default:
                break;
        }
    }

    return 0;
}
