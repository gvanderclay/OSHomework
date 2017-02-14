CC = gcc
CCFLAGS = -Wall

all: reader.out writer.out

reader.out: key
	$(CC) $(CCFLAGS) -c reader.c
	$(CC) $(CCFLAGS) -o reader.out reader.o key.o

writer.out: key
	$(CC) $(CCFLAGS) -c writer.c
	$(CC) $(CCFLAGS) -o writer.out writer.o key.o

key:
	$(CC) $(CCFLAGS) -c key.c

clean:
	rm -rf *.out *.o
