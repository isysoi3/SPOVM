#include <iostream>
#include <windows.h>
#include <string>
#define DELAY_TIME_SEC 2

int main(int argc, char *argv[])
{
    HANDLE hEndEvent = OpenEvent(SYNCHRONIZE, FALSE, argv[1]);
    if (hEndEvent == NULL)
    {
        std::cout << "Open event failed.\n";
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
        for(std::string::size_type i = 0; i < processString.size(); ++i) {
            std::cout << processString[i];
        } 
        std::cout << std::endl;
       
        Sleep(DELAY_TIME_SEC * 1000);
    }

    CloseHandle(hEndEvent);
    return 0;
}