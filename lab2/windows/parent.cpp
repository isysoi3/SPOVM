#include <iostream>
#include <stack>
#include <windows.h>


std::stack <HANDLE> childHandles; 

void printMenu() {
    std::cout << "Child processes: " << childHandles.size() << std::endl;
    std::cout << "[+] - add process\n";
    std::cout << "[-] - remove process\n";
    std::cout << "[q] - exit\n";
}

void addChildProcess() {
    std::cout << "Add new child process will be here\n";
}

void removeChildProcess() {
     std::cout << "Remove child process will be here\n";
}

void removeAllChildProcess() {
    std::cout << "Add all child processes will be here\n";
    while(!childHandles.empty()) {
        // pid_t childPid = childPids.top();
        // childPids.pop();
        // kill(childPid, SIGKILL);
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