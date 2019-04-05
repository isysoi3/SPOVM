#include <iostream>
#include <windows.h>
#define DELAY_TIME_SEC 2

int main() {
    while (true) {
        std::cout << "Hello world!" << std::endl;
        Sleep(DELAY_TIME_SEC * 1000);
    }
    return 0;
}