CC=gcc
CFLAGS=-fPIC
LDFLAGS=-fPIC -shared
SOURCES=ahmalloc.c bssalloc.c
OBJECTS=$(SOURCES:.c=.o)
EXECUTABLE=ahalloc.so

all: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(LDFLAGS) -o $(EXECUTABLE) $(OBJECTS)

.c.o:
	$(CC) $(CFLAGS) -c $<

clean:
	rm -f *.o $(EXECUTABLE)
