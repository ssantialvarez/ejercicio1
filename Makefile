CC=gcc
CFLAGS=-g -Wall
SRC=$(wildcard *.c)
OBJ=$(SRC:.c=.o)
EXEC=main

all: $(EXEC)

$(EXEC): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f *.o $(EXEC)
