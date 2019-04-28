#include <iostream>
#include <Windows.h>
#include <string.h>

#define CLIENT_END_EVENT_NAME "CLIENT_END_EVENT"
#define PIPE_NAME "\\\\.\\pipe\\server_pipe"

#define LINE_LEN 512

int main()
{
    HANDLE clientEndEvent = OpenEvent(EVENT_ALL_ACCESS, FALSE, CLIENT_END_EVENT_NAME);
    if (clientEndEvent == NULL)
    {
        std::cout << "Opening client end event failed." << '\n';
        return GetLastError();
    }

    HANDLE pipe = CreateFile(PIPE_NAME, GENERIC_READ, FILE_SHARE_READ,NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    
    if(pipe == INVALID_HANDLE_VALUE)
    {
        std::cout << "Opening pipe failed." << '\n';
        CloseHandle(clientEndEvent);
        return GetLastError();
    }


    char str [LINE_LEN];
    DWORD dwBytesRead;
    while(true){
        if (!ReadFile(pipe,  str, sizeof(str), &dwBytesRead, NULL))  {   
            CloseHandle(pipe);   
            return GetLastError();  
        }
        std::cout << str << '\n';
        SetEvent(clientEndEvent);
        if (strcmp(str, "q") == 0)
        {
            break;
        }
    }
    
    std::cout << "Client process finish work.";
    CloseHandle(clientEndEvent);
    return 0;
}