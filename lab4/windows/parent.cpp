#include <iostream>
#include <stack>
#include <windows.h>
#include <vector>
#include <string>

#define BUF_SIZE 256
#define DELAY_TIME_SEC 2

CRITICAL_SECTION cs;
std::stack<HANDLE> childThreads;
std::stack<HANDLE> childEndEvents;

void printMenu()
{
    EnterCriticalSection (&cs); 
    std::cout << "Child threads: " << childThreads.size() << "\n";
    std::cout << "[+] - add thread\n";
    std::cout << "[-] - remove thread\n";
    std::cout << "[q] - exit\n";
    LeaveCriticalSection(&cs); 
}

DWORD WINAPI Add(LPVOID endEvent)
{
    HANDLE hEndEvent = (HANDLE)endEvent;
    DWORD threadId = GetCurrentThreadId();
    std::string processString = "Child process id - ";
    processString.append(std::to_string(threadId));
    while (true)
    {
        DWORD dwWaitResult = WaitForSingleObject(hEndEvent, 0);
        if (dwWaitResult == WAIT_OBJECT_0)
        {
            break;
        }
        EnterCriticalSection (&cs); 
        for (std::string::size_type i = 0; i < processString.size(); ++i)
        {
            std::cout << processString[i];
        }
        std::cout << std::endl;
        LeaveCriticalSection(&cs); 
        Sleep(DELAY_TIME_SEC * 1000);
    }
    return 0;
}

void addChildThread()
{
    HANDLE hEndEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
    if (hEndEvent == NULL) {
        std::cout << "Error were occurred while creating closing event for new thread.\n";
        return;
    }
    DWORD IDThread;
    HANDLE hThread = CreateThread(NULL, 0, Add, (void *)hEndEvent, 0, &IDThread);
    if (hThread == NULL) {
        std::cout << "Error were occurred while creating new thread.\n";
        CloseHandle(hEndEvent);
        return;
    }
    childEndEvents.push(hEndEvent);
    childThreads.push(hThread);
}

void killThread()
{
    HANDLE hThread = childThreads.top();
    childThreads.pop();
    HANDLE hEndEvent = childEndEvents.top();
    childEndEvents.pop();
    SetEvent(hEndEvent);
    WaitForSingleObject(hThread, INFINITE);
    CloseHandle(hEndEvent);
    CloseHandle(hThread);
}

void removeChildProcess()
{
    if (childThreads.empty())
    {
        EnterCriticalSection (&cs); 
        std::cout << "Nothing to delete.\n";
        LeaveCriticalSection(&cs);
    }
    else
    {
        killThread();
    }
}

void removeAllChildProcess()
{
    while (!childThreads.empty())
    {
        killThread();
    }
}

int main()
{
    char operation;
    bool isFinished = false;
    InitializeCriticalSection(&cs); 
    while (!isFinished)
    {
        printMenu();
        std::cin >> operation;
        std::cin.ignore();
        switch (operation)
        {
        case '+':
            addChildThread();
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
    DeleteCriticalSection(&cs); 
    return 0;
}