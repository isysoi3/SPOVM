#include <iostream>
#include <Windows.h>
#include <string.h>

#define SEMAPHORE_START_READING_NAME "SEM_START"
#define SEMAPHORE_END_READING_NAME "SEM_END"
#define CLIENT_END_EVENT_NAME "CLIENT_END_EVENT"
#define PIPE_NAME "\\\\.\\pipe\\server_pipe"

#define LINE_LEN 512

int main()
{
    HANDLE semaphoreStartReading = OpenSemaphore(SEMAPHORE_ALL_ACCESS , NULL,  SEMAPHORE_START_READING_NAME); 
    if (semaphoreStartReading == NULL)
    {
        std::cout << "Opening semaphore start reading failed." << '\n';
        return GetLastError();
    }

    HANDLE semaphoreEndReading = OpenSemaphore(SEMAPHORE_ALL_ACCESS , NULL, SEMAPHORE_END_READING_NAME); 
    if (semaphoreEndReading == NULL)
    {
        std::cout << "Opening semaphore end reading failed." << '\n';
        return GetLastError();
    }

    HANDLE pipe = CreateFile(PIPE_NAME, GENERIC_READ, FILE_SHARE_READ,NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    if(pipe == INVALID_HANDLE_VALUE)
    {
        std::cout << "Opening pipe failed." << '\n';
        CloseHandle(semaphoreStartReading);
        return GetLastError();
    }

    char str [LINE_LEN];
    DWORD dwBytesRead;
    WaitForSingleObject(semaphoreEndReading, INFINITE); 
    while(true) {
        ReleaseSemaphore(semaphoreEndReading, 1, NULL); 
        WaitForSingleObject(semaphoreStartReading, INFINITE); 
        std::cout << "Client process get string : ";
        if (!ReadFile(pipe,  str, sizeof(str), &dwBytesRead, NULL))  {   
            CloseHandle(pipe);   
            return GetLastError();  
        }
        std::cout << str << '\n';
        ReleaseSemaphore(semaphoreStartReading, 1, NULL); 
        WaitForSingleObject(semaphoreEndReading, INFINITE); 
        if (strcmp(str, "q") == 0)
        {
            break;
        }
    }
    
    std::cout << "Client process finish work.";
    CloseHandle(semaphoreStartReading);
    CloseHandle(semaphoreEndReading);
    return 0;
}