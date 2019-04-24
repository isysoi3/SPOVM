#include <iostream>
#include <stack> 
#include <sys/types.h>
#include <signal.h>
#include <unistd.h>
#include <termios.h>
#include <unistd.h>
#include <stdio.h>

#define DELAY_TIME_SEC 2

std::stack <pid_t> childPids; 

int getch() {
    struct termios oldattr, newattr;
    int ch;
    tcgetattr( STDIN_FILENO, &oldattr );
    newattr = oldattr;
    newattr.c_lflag &= ~( ICANON | ECHO );
    tcsetattr( STDIN_FILENO, TCSANOW, &newattr );
    ch = getchar();
    tcsetattr( STDIN_FILENO, TCSANOW, &oldattr );
    return ch;
}

void killPid(pid_t pid) {
     if (kill(pid, SIGTERM) == -1) {
          kill(pid, SIGKILL);
     }
}

void printMenu() {
    std::cout << "Child processes: " << childPids.size() << std::endl;
    std::cout << "[+] - add process\n";
    std::cout << "[-] - remove process\n";
    std::cout << "[q] - exit\n";
}

void removeChildProcess() {
    if (childPids.empty()) {
        std::cout << "Nothing to delete.\n";
    } else {
        pid_t childPid = childPids.top();
        childPids.pop();
        killPid(childPid);
    }
}

void removeAllChildProcess() {
    while(!childPids.empty()) {
        pid_t childPid = childPids.top();
        childPids.pop();
        killPid(childPid);
    }
}


int main() {
    char operation;
    bool isFinished = false;
    pid_t pid;

    while(!isFinished) {
        printMenu();
        operation = getch();
        switch (operation) {
            case '+':
                pid = fork();
                switch(pid) {
                    case -1: 
                        //failed to frok
                        break;
                    case 0: //child
                        execv("child.out", NULL);
                        return 0;
                    default: //parent
                        childPids.push(pid);
                        break;
                }
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
