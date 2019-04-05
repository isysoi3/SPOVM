#include <iostream>
#include <stack>
#include <windows.h>

#define CHILD_PROCESS_NAME "child.exe"
#define BUF_SIZE 256

std::stack<std::pair<PROCESS_INFORMATION, HANDLE>> childProcesses;

int newProcessNum = 0;

void printMenu()
{
    std::cout << "Child processes: " << childProcesses.size() << "\n";
    std::cout << "[+] - add process\n";
    std::cout << "[-] - remove process\n";
    std::cout << "[q] - exit\n";
}

HANDLE createEvent()
{
    char endEventName[BUF_SIZE] = "";
    sprintf(endEventName, "%d", newProcessNum);
    HANDLE handleEndEvent = CreateEvent(NULL, FALSE, FALSE, endEventName);
    if (handleEndEvent == NULL)
    {
        std::cout << "Failed to create end event for new process";
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
        std::cout << "Failed to create new process\n";
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
        std::cout << "Nothing to delete.\n";
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

    return 0;
}