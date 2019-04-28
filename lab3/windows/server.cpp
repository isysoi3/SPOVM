#include <iostream>
#include <Windows.h>
#include <string.h>

#define SEMAPHORE_START_READING_NAME "SEM_START"
#define SEMAPHORE_END_READING_NAME "SEM_END"
#define PIPE_NAME "\\\\.\\pipe\\server_pipe"
#define CLIENT_PROCESS "client.exe"

#define LINE_LEN 512

int main()
{
    HANDLE semaphoreStartReading = CreateSemaphore(NULL, 1, 1, SEMAPHORE_START_READING_NAME);
    if (semaphoreStartReading == NULL)
    {
        std::cout << "Creating semaphore start reading failed." << '\n';
        return GetLastError();
    }
    WaitForSingleObject(semaphoreStartReading, INFINITE); 

    HANDLE semaphoreEndReading = CreateSemaphore(NULL, 1, 1, SEMAPHORE_END_READING_NAME);
    if (semaphoreEndReading == NULL)
    {
        std::cout << "Creating semaphore end reading failed." << '\n';
        CloseHandle(semaphoreStartReading);
        return GetLastError();
    }

    HANDLE pipe = CreateNamedPipe(PIPE_NAME, PIPE_ACCESS_DUPLEX, PIPE_TYPE_MESSAGE | PIPE_WAIT, 1, 0, 0, INFINITE, NULL);
    if (pipe == INVALID_HANDLE_VALUE)
    {
        std::cout << "Creating pipe failed." << '\n';
        CloseHandle(semaphoreStartReading);
        return GetLastError();
    }

    STARTUPINFO si;  
    PROCESS_INFORMATION piApp; 
    ZeroMemory(&si, sizeof(STARTUPINFO));  
    si.cb = sizeof(STARTUPINFO); 
    if (!CreateProcess(CLIENT_PROCESS, NULL, NULL, NULL, FALSE, CREATE_NEW_CONSOLE, NULL, NULL, &si, &piApp))
    {
        std::cout << "Error were occurred while creating child process.";
        CloseHandle(semaphoreStartReading);
        CloseHandle(pipe);
        return 0;
    }

    std::cout << "Server is waiting client connection...\n";
    if (!ConnectNamedPipe(pipe, NULL))
    {
        std::cout << "Client doesn't connected to server!";
        CloseHandle(piApp.hThread);  
        CloseHandle(piApp.hProcess); 
        CloseHandle(semaphoreStartReading);
        CloseHandle(pipe);
        return GetLastError();
    }

    char str[LINE_LEN];
    DWORD dwBytesWrite;
    while (true)
    {
        std::cout << "To finish server process enter q. Enter string: ";
        std::cin.getline(str, LINE_LEN);
        ReleaseSemaphore(semaphoreStartReading, 1, NULL); 
        WaitForSingleObject(semaphoreEndReading, INFINITE);
        if (!WriteFile(pipe, str, sizeof(str), &dwBytesWrite, NULL))
        {
            std::cout << "Error were occurred while writing to pipe.";
            CloseHandle(piApp.hThread);  
            CloseHandle(piApp.hProcess); 
            CloseHandle(semaphoreStartReading);
            CloseHandle(pipe);
            return GetLastError();
        }

        std::cout << "Send string to client process...\n";
        WaitForSingleObject(semaphoreStartReading, INFINITE);
        ReleaseSemaphore(semaphoreEndReading, 1, NULL); 
        if (strcmp(str, "q") == 0)
        {
            break;
        }
    }
    std::cout << "Server process finish work.";
    WaitForSingleObject(piApp.hProcess, INFINITE);
    CloseHandle(piApp.hThread);  
    CloseHandle(piApp.hProcess); 
    CloseHandle(semaphoreStartReading);
    CloseHandle(semaphoreEndReading);
    CloseHandle(pipe);
    return 0;
}