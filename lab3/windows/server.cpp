#include <iostream>
#include <Windows.h>
#include <string.h>

#define CLIENT_END_EVENT_NAME "CLIENT_END_EVENT"
#define PIPE_NAME "\\\\.\\pipe\\server_pipe"
#define CLIENT_PROCESS "client.exe"

#define LINE_LEN 512

int main()
{
    HANDLE clientEndEvent = CreateEvent(NULL, FALSE, FALSE, CLIENT_END_EVENT_NAME);
    if (clientEndEvent == NULL)
    {
        std::cout << "Creating client end event failed." << '\n';
        return GetLastError();
    }

    HANDLE pipe = CreateNamedPipe(PIPE_NAME, PIPE_ACCESS_DUPLEX, PIPE_TYPE_MESSAGE | PIPE_WAIT, 1, 0, 0, INFINITE, NULL);
    if (pipe == INVALID_HANDLE_VALUE)
    {
        std::cout << "Creating pipe failed." << '\n';
        CloseHandle(clientEndEvent);
        return GetLastError();
    }

    STARTUPINFO si;  
    PROCESS_INFORMATION piApp; 
    ZeroMemory(&si, sizeof(STARTUPINFO));  
    si.cb = sizeof(STARTUPINFO); 
    if (!CreateProcess(CLIENT_PROCESS, NULL, NULL, NULL, FALSE, CREATE_NEW_CONSOLE, NULL, NULL, &si, &piApp))
    {
        std::cout << "Error were occurred while creating child process.";
        CloseHandle(clientEndEvent);
        CloseHandle(pipe);
        return 0;
    }

    std::cout << "Server is waiting client connection...\n";
    if (!ConnectNamedPipe(pipe, NULL))
    {
        std::cout << "Client doesn't connected to server!";
        CloseHandle(piApp.hThread);  
        CloseHandle(piApp.hProcess); 
        CloseHandle(clientEndEvent);
        CloseHandle(pipe);
        return GetLastError();
    }

    char str[LINE_LEN];
    DWORD dwBytesWrite;
    while (true)
    {
        std::cout << "To finish server process enter q. Enter string: ";
        std::cin.getline(str, LINE_LEN);
        if (!WriteFile(pipe, str, sizeof(str), &dwBytesWrite, NULL))
        {
            std::cout << "Error were occurred while writing to pipe.";
            CloseHandle(piApp.hThread);  
            CloseHandle(piApp.hProcess); 
            CloseHandle(clientEndEvent);
            CloseHandle(pipe);
            return GetLastError();
        }

        std::cout << "Send string to client process...\n";
        WaitForSingleObject(clientEndEvent, INFINITE);
        if (strcmp(str, "q") == 0)
        {
            break;
        }
    }
    std::cout << "Server process finish work.";
    WaitForSingleObject(piApp.hProcess, INFINITE);
    CloseHandle(piApp.hThread);  
    CloseHandle(piApp.hProcess); 
    CloseHandle(clientEndEvent);
    CloseHandle(pipe);
    return 0;
}