#include <iostream>
#include <unistd.h>

#define DELAY_TIME_SEC 2

int main() {
    while (true) {
        std::cout << "Process id - " << getpid() << std::endl;
        sleep(DELAY_TIME_SEC);
    }
    return 0;
}
