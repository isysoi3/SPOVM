CC=g++
CFLAGS=-c 
SOURCES=linux_processes.cpp
OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE=linux_processes
LDFLAGS=-lncurses

all: $(SOURCES) $(EXECUTABLE)
	
$(EXECUTABLE): $(OBJECTS) 
	$(CC) $(OBJECTS) -o $@ $(LDFLAGS)

.cpp.o:
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -rf *.o $(EXECUTABLE)