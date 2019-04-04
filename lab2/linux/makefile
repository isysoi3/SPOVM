CC=g++
CFLAGS=-c 
SOURCES=parent.cpp
OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE=parent
LDFLAGS=-lncurses

all: $(SOURCES) $(EXECUTABLE)
	
$(EXECUTABLE): $(OBJECTS) 
	$(CC) $(OBJECTS) -o $@ $(LDFLAGS)

.cpp.o:
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -rf *.o $(EXECUTABLE)