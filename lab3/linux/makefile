CC=g++
CFLAGS=-c 
SOURCES=server.cpp client.cpp
OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE=server client
LDFLAGS=

all: $(SOURCES) $(EXECUTABLE)
	
$(EXECUTABLE): $(OBJECTS) 
	$(CC) $@.o -o $@.out $(LDFLAGS)

.cpp.o:
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -rf *.o $(EXECUTABLE)
