CC = gcc
CCFLAGS = -Wall

all: reader.out writer.out

reader.out: sharedMem
	$(CC) $(CCFLAGS) -c reader.c
	$(CC) $(CCFLAGS) -o reader.out reader.o sharedMem.o

writer.out: sharedMem
	$(CC) $(CCFLAGS) -c writer.c
	$(CC) $(CCFLAGS) -o writer.out writer.o sharedMem.o

sharedMem:
	$(CC) $(CCFLAGS) -c sharedMem.c

clean:
	rm -rf *.out *.o
