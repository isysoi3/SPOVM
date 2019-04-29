#include <iostream>
#include <Windows.h>
#include <string.h>

#define SEMAPHORE_READ_NAME "SEM_READ"
#define SEMAPHORE_WRITE_NAME "SEM_WRITE"
#define CLIENT_END_EVENT_NAME "CLIENT_END_EVENT"
#define PIPE_NAME "\\\\.\\pipe\\server_pipe"

#define LINE_LEN 512

void closeHandles(HANDLE semRead, HANDLE semWrite, HANDLE pipe) {
    CloseHandle(semWrite);
    CloseHandle(semRead);
    CloseHandle(pipe);
}

int main()
{
    HANDLE semaphoreRead = OpenSemaphore(SEMAPHORE_ALL_ACCESS, NULL, SEMAPHORE_READ_NAME);
    if (semaphoreRead == NULL)
    {
        std::cout << "Opening semaphore start reading failed." << '\n';
        return GetLastError();
    }
    WaitForSingleObject(semaphoreRead, INFINITE); 

    HANDLE semaphoreWrite = OpenSemaphore(SEMAPHORE_ALL_ACCESS, NULL, SEMAPHORE_WRITE_NAME);
    if (semaphoreWrite == NULL)
    {
        std::cout << "Opening semaphore end reading failed." << '\n';
        CloseHandle(semaphoreRead);
        return GetLastError();
    }

    HANDLE pipe = CreateFile(PIPE_NAME, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    if (pipe == INVALID_HANDLE_VALUE)
    {
        std::cout << "Opening pipe failed." << '\n';
        CloseHandle(semaphoreWrite);
        CloseHandle(semaphoreRead);
        return GetLastError();
    }

    char str[LINE_LEN];
    DWORD dwBytesRead;

    while (true)
    {
        WaitForSingleObject(semaphoreWrite, INFINITE);
        ReleaseSemaphore(semaphoreRead, 1, NULL);
        std::cout << "Client woken up.\n";
        if (!ReadFile(pipe, str, sizeof(str), &dwBytesRead, NULL))
        {
            closeHandles(semaphoreRead, semaphoreWrite, pipe);
            return GetLastError();
        }
        std::cout << "Accepted string from server process: " << str << '\n';
        ReleaseSemaphore(semaphoreWrite, 1, NULL);
        WaitForSingleObject(semaphoreRead, INFINITE);
        if (strcmp(str, "q") == 0)
        {
            break;
        }
    }

    closeHandles(semaphoreRead, semaphoreWrite, pipe);
    return 0;
}