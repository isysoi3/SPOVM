#include <iostream>
#include <stack>
#include <windows.h>

#define CHILD_PROCESS_NAME "child.exe"
#define BUF_SIZE 256
#define MUTEX_CONSOLE_NAME "console_mutex"

std::stack<std::pair<PROCESS_INFORMATION, HANDLE>> childProcesses;

int newProcessNum = 0;
HANDLE consoleMutex;

void printMenu()
{
    WaitForSingleObject(consoleMutex, INFINITE);
    std::cout << "Child processes: " << childProcesses.size() << "\n";
    std::cout << "[+] - add process\n";
    std::cout << "[-] - remove process\n";
    std::cout << "[q] - exit\n";
    ReleaseMutex(consoleMutex);
}

HANDLE createEvent()
{
    char endEventName[BUF_SIZE] = "";
    sprintf(endEventName, "%d", newProcessNum);
    HANDLE handleEndEvent = CreateEvent(NULL, FALSE, FALSE, endEventName);
    if (handleEndEvent == NULL)
    {
        WaitForSingleObject(consoleMutex, INFINITE);
        std::cout << "Failed to create end event for new process";
        ReleaseMutex(consoleMutex);
    }
    return handleEndEvent;
}

void addChildProcess()
{
    HANDLE handleEndEvent = createEvent();
    if (handleEndEvent == NULL)
    {
        return;
    }
    char childProcessArgs[BUF_SIZE] = "";
    sprintf(childProcessArgs, "%s %d", CHILD_PROCESS_NAME, newProcessNum);
    STARTUPINFO si;
    PROCESS_INFORMATION piApp;
    ZeroMemory(&si, sizeof(STARTUPINFO));
    si.cb = sizeof(STARTUPINFO);
    if (!CreateProcess(NULL, childProcessArgs, NULL, NULL, FALSE, NULL, NULL, NULL, &si, &piApp))
    {
        WaitForSingleObject(consoleMutex, INFINITE);
        std::cout << "Failed to create new process\n";
        ReleaseMutex(consoleMutex);
        return;
    }
    newProcessNum++;
    childProcesses.push(std::make_pair(piApp, handleEndEvent));
}

void killProcess(std::pair<PROCESS_INFORMATION, HANDLE> childProcess)
{
    SetEvent(childProcess.second);
    WaitForSingleObject(childProcess.first.hProcess, INFINITE);
    CloseHandle(childProcess.second);
    CloseHandle(childProcess.first.hThread);
    CloseHandle(childProcess.first.hProcess);
}

void removeChildProcess()
{
    if (childProcesses.empty())
    {
        WaitForSingleObject(consoleMutex, INFINITE);
        std::cout << "Nothing to delete.\n";
        ReleaseMutex(consoleMutex);
    }
    else
    {
        std::pair<PROCESS_INFORMATION, HANDLE> childProcess = childProcesses.top();
        childProcesses.pop();
        killProcess(childProcess);
    }
}

void removeAllChildProcess()
{
    while (!childProcesses.empty())
    {
        std::pair<PROCESS_INFORMATION, HANDLE> childProcess = childProcesses.top();
        childProcesses.pop();
        killProcess(childProcess);
    }
}

int main()
{
    char operation;
    bool isFinished = false;

    consoleMutex = CreateMutex(NULL, FALSE, MUTEX_CONSOLE_NAME);
    if (consoleMutex == NULL)
    {
        std::cout << "Create mutext failed.\n";
        return GetLastError();
    }
    while (!isFinished)
    {
        printMenu();
        std::cin >> operation;
        std::cin.ignore();
        switch (operation)
        {
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

    CloseHandle(consoleMutex);
    return 0;
}