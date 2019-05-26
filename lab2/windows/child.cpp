#include <iostream>
#include <windows.h>
#include <string>

#define DELAY_TIME_SEC 2
#define MUTEX_CONSOLE_NAME "console_mutex"

int main(int argc, char *argv[])
{
    HANDLE hEndEvent = OpenEvent(SYNCHRONIZE, FALSE, argv[1]);
    if (hEndEvent == NULL)
    {
        std::cout << "Open event failed.\n";
        return GetLastError();
    }
    HANDLE consoleMutex = OpenMutex(SYNCHRONIZE, FALSE, MUTEX_CONSOLE_NAME);
    if (consoleMutex == NULL)
    {
        std::cout << "Open mutext failed.\n";
        CloseHandle(hEndEvent);
        return GetLastError();
    }

    std::string processString = "Child process id - ";
    processString.append(argv[1]);
    while (true)
    {
        DWORD dwWaitResult = WaitForSingleObject(hEndEvent, 0);
        if (dwWaitResult == WAIT_OBJECT_0)
        {
            break;
        }
        WaitForSingleObject(consoleMutex, INFINITE);
        for (std::string::size_type i = 0; i < processString.size(); ++i)
        {
            std::cout << processString[i];
        }
        std::cout << std::endl;
        ReleaseMutex(consoleMutex);
        Sleep(DELAY_TIME_SEC * 1000);
    }

    CloseHandle(hEndEvent);
    CloseHandle(consoleMutex);
    return 0;
}