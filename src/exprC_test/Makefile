CC=gcc
CFLAGS=-std=gnu99 -Wall -Wextra -pedantic
BIN=test
SOURCE=ExprC.c

all:
	$(CC) $(CFLAGS) $(SOURCE) -o $(BIN)
run: all
	./$(BIN)
clean:
	rm $(BIN)
