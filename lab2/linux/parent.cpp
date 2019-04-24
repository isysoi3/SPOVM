#include <iostream>
#include <stack> 
#include <sys/types.h>
#include <signal.h>
#include <unistd.h>

#define DELAY_TIME_SEC 2

std::stack <pid_t> childPids; 

void printMenu() {
    std::cout << "Child processes: " << childPids.size() << std::endl;
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
            execv("child.out", NULL);
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
        std::cin.ignore();
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
