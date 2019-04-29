#include <iostream>
#include <Windows.h>
#include <string.h>

#define SEMAPHORE_READ_NAME "SEM_READ"
#define SEMAPHORE_WRITE_NAME "SEM_WRITE"
#define PIPE_NAME "\\\\.\\pipe\\server_pipe"
#define CLIENT_PROCESS "client.exe"

#define LINE_LEN 512

void closeHandles(HANDLE semRead, HANDLE semWrite, HANDLE pipe, PROCESS_INFORMATION piApp) {
    CloseHandle(piApp.hThread);  
    CloseHandle(piApp.hProcess); 
    CloseHandle(semWrite);
    CloseHandle(semRead);
    CloseHandle(pipe);
}

int main()
{
    HANDLE semaphoreRead = CreateSemaphore(NULL, 1, 1, SEMAPHORE_READ_NAME);
    if (semaphoreRead == NULL)
    {
        std::cout << "Creating semaphore start reading failed." << '\n';
        return GetLastError();
    }

    HANDLE semaphoreWrite = CreateSemaphore(NULL, 1, 1, SEMAPHORE_WRITE_NAME);
    if (semaphoreWrite == NULL)
    {
        std::cout << "Creating semaphore end reading failed." << '\n';
        CloseHandle(semaphoreRead);
        return GetLastError();
    }
    WaitForSingleObject(semaphoreWrite, INFINITE);

    HANDLE pipe = CreateNamedPipe(PIPE_NAME, PIPE_ACCESS_DUPLEX, PIPE_TYPE_MESSAGE | PIPE_WAIT, 1, 0, 0, INFINITE, NULL);
    if (pipe == INVALID_HANDLE_VALUE)
    {
        std::cout << "Creating pipe failed." << '\n';
        CloseHandle(semaphoreWrite);
        CloseHandle(semaphoreRead);
        return GetLastError();
    }

    STARTUPINFO si;  
    PROCESS_INFORMATION piApp; 
    ZeroMemory(&si, sizeof(STARTUPINFO));  
    si.cb = sizeof(STARTUPINFO); 
    if (!CreateProcess(CLIENT_PROCESS, NULL, NULL, NULL, FALSE, CREATE_NEW_CONSOLE, NULL, NULL, &si, &piApp))
    {
        std::cout << "Error were occurred while creating child process.";
        CloseHandle(semaphoreWrite);
        CloseHandle(semaphoreRead);
        CloseHandle(pipe);
        return 0;
    }

    std::cout << "Server is waiting client connection...\n";
    if (!ConnectNamedPipe(pipe, NULL))
    {
        std::cout << "Client doesn't connected to server!";
        closeHandles(semaphoreRead, semaphoreWrite, pipe, piApp);
        return GetLastError();
    }

    char str[LINE_LEN];
    DWORD dwBytesWrite;
    while (true)
    {
        std::cout << "To finish server process enter q. Enter string: ";
        std::cin.getline(str, LINE_LEN);
        ReleaseSemaphore(semaphoreWrite, 1, NULL); 
        WaitForSingleObject(semaphoreRead, INFINITE);
        if (!WriteFile(pipe, str, sizeof(str), &dwBytesWrite, NULL))
        {
            std::cout << "Error were occurred while writing to pipe.";
            closeHandles(semaphoreRead, semaphoreWrite, pipe, piApp);
            return GetLastError();
        }
        std::cout << "Send string to client process: " << str << '\n';
        WaitForSingleObject(semaphoreWrite, INFINITE);
        ReleaseSemaphore(semaphoreRead, 1, NULL); 
        if (strcmp(str, "q") == 0)
        {
            break;
        }
    }

    WaitForSingleObject(piApp.hProcess, INFINITE);
    closeHandles(semaphoreRead, semaphoreWrite, pipe, piApp);
    return 0;
}