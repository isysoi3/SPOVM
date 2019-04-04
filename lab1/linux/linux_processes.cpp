#include <ctime>    
#include <unistd.h>
#include <ncurses.h>
#include <sstream>
#include <iostream>
#include <string>
#include <cstring>
#include <sys/wait.h>
#include <sys/mman.h>
#include <cstdlib>
#define ARRAY_SIZE 10
#define DELAY_TIME_SEC 5

void printArray(WINDOW *win, int y, int* array, const char* id){
	mvwprintw(win, y, 0, "id - %s", id);
	std::stringstream ss;
	for(int i = 0; i < ARRAY_SIZE; i++) {
    	ss << array[i] << " ";
    }
    mvwprintw(win, y + 1, 0, "%s", ss.str().c_str());
    mvwprintw(win, y + 2, 0, "%s", "\n");
    wrefresh(win);
}

int exitWithFaildAndMessage(const char * message) {
	printw(message);
    getch();
    endwin();
    return -1;
}

int main() {
    WINDOW* win = initscr();
    noecho();
    curs_set(0);
    refresh();

	int* array;
    array = static_cast<int*>(mmap(NULL, 
                ARRAY_SIZE*sizeof(int),
                PROT_READ | PROT_WRITE, 
                MAP_ANONYMOUS | MAP_SHARED,
                -1,
                0));

	srand(time(0));
    int pid = fork(); 
    switch(pid) {
    case -1: 
    	return exitWithFaildAndMessage("failed to fork"); 
    case 0: 
		for (int i = 0; i< ARRAY_SIZE; i++)
	    	array[i] = 1 + rand() % 100;
		printArray(win, 0, array, "child");

        sleep(DELAY_TIME_SEC);
    	break;	
    default:
    	while(wait(NULL) > 0);
		printArray(win, 1, array, "parent");

		getch();
		munmap(array, ARRAY_SIZE);
		endwin();
        break;
    }

    return 0;
}
