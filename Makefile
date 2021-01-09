CC=gcc
CLINK=$(CC) # Read variable CC using $(CC)
CFLAGS=-g -Wall -std=c99
RM=rm -rf *.o *.exe libgrades.so

libgrades.so: grades.o
	$(CLINK) -shared grades.o -o libgrades.so -llinked-list -L.

grades.o: grades.c grades.h linked-list.h
	$(CC) $(CFLAGS) -c -fpic grades.c

clean:
	$(RM)
