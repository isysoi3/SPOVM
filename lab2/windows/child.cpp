#include <iostream>
#include <windows.h>
#define DELAY_TIME_SEC 2

int main(int argc, char *argv[])
{
    HANDLE hEndEvent = OpenEvent(SYNCHRONIZE, FALSE, argv[1]);
    if (hEndEvent == NULL)
    {
        std::cout << "Open event failed.\n";
        return GetLastError();
    }

    while (true)
    {
        DWORD dwWaitResult = WaitForSingleObject(hEndEvent, 0);
        if (dwWaitResult == WAIT_OBJECT_0)
        {
            break;
        }
        std::cout << "Child process string: " << argv[1] << '\n';
        Sleep(DELAY_TIME_SEC * 1000);
    }

    CloseHandle(hEndEvent);
    return 0;
}