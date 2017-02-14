CC=gcc
CCFLAGS=-Wall
LDFLAGS=
SOURCES=$(wildcard *.c)
OBJECTS=$(SOURCES:.c=.o)
TARGET=des

all: $(TARGET)

%.o: %.c %.h
	$(CC) $(CCFLAGS) $<

%.o: %.c
	$(CC) $(CCFLAGS) $<

clean:
	rm -f *.o $(TARGET)
