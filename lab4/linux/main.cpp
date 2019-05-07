#include <stack>
#include <iostream>
#include <termios.h>
#include <unistd.h>
#include <stdio.h>
#include "pthread.h"

#define DELAY_TIME_SEC 2

std::stack <pthread_t> childThreads; 
pthread_mutex_t consoleMutex;

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

void printMenu() {
    pthread_mutex_lock(&consoleMutex);
    std::cout << "Child threads: " << childThreads.size() << std::endl;
    std::cout << "[+] - add child thread\n";
    std::cout << "[-] - remove child thread\n";
    std::cout << "[q] - exit\n";
    pthread_mutex_unlock(&consoleMutex);
}

void killChildThread(pthread_t childThread) {
    pthread_cancel(childThread);
}

void* childThreadFunc(void * args) {
    std::string processString = "Child thread id - " + std::to_string((long) pthread_self());
    while (true) {
        pthread_mutex_lock(&consoleMutex);
        for(std::string::size_type i = 0; i < processString.size(); ++i) {
            std::cout << processString[i];
        } 
        std::cout << std::endl;
        pthread_mutex_unlock(&consoleMutex);
        sleep(DELAY_TIME_SEC);
    }
}

void addChildThread() {
	pthread_t thread;
	int threadCreateResult = pthread_create(&thread, NULL, childThreadFunc, NULL);
    if (threadCreateResult == 0) {
        childThreads.push(thread);
    } else {
        std::cout << "Failed to create child thread\n";
    }
}

void removeChildThread() {
    if (childThreads.empty()) {
        std::cout << "Nothing to delete.\n";
    } else {
        pthread_t childThread = childThreads.top();
        childThreads.pop();
        killChildThread(childThread);
    }
}

void removeAllChildThreads() {
    while(!childThreads.empty()) {
        pthread_t childThread = childThreads.top();
        childThreads.pop();
        killChildThread(childThread);
    }
}

int main() {
    char operation;
    bool isFinished = false;

	pthread_mutex_init(&consoleMutex, NULL);
    while(!isFinished) {
        printMenu();
        operation = getch();
        switch (operation) {
            case '+':
                addChildThread();
                break;
            case '-':
                removeChildThread();
                break;
            case 'q':
                removeAllChildThreads();
                isFinished = true;
                break;
            default:
                break;
        }
    }
    pthread_mutex_destroy(&consoleMutex);

    return 0;
}
