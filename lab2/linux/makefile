CC=g++
CFLAGS=-c 
SOURCES=parent.cpp child.cpp
OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE=parent child
LDFLAGS=

all: $(SOURCES) $(EXECUTABLE)
	
$(EXECUTABLE): $(OBJECTS) 
	$(CC) $@.o -o $@.out $(LDFLAGS)

.cpp.o:
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -rf *.o $(EXECUTABLE)