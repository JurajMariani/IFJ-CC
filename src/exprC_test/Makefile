CC=gcc
CFLAGS=-Wall -Wextra -Werror -std=c99
CodeFiles= ../codes/BubbleStack.c ../codes/expressionCommander.c ../codes/TS.c
TestFiles= ExprC.c
OutputName= test


.PHONY= all run code

all:
	$(CC) $(CFLAGS) $(CodeFiles) $(TestFiles) -o $(OutputName)


run: all
	./$(OutputName)


code: 
	$(CC) $(CFLAGS) $(CodeFiles) -o tmp_result


