#include <iostream>
#include <unistd.h>
#include <string>
#include <signal.h>

#define DELAY_TIME_SEC 2

int main() {
    std::string processString = "Process id - " + std::to_string(getpid());
    while (true) {
        for(std::string::size_type i = 0; i < processString.size(); ++i) {
            std::cout << processString[i];
        } 
        std::cout << std::endl;
        sleep(DELAY_TIME_SEC);
    }
    return 0;
}
