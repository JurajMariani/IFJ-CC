CC=gcc
CFLAGS=-Wall -Wextra -std=c99
CodeFiles= ../codes/err_one_func.c ../codes/DebugHelp.c ../codes/BubbleStack.c ../codes/expressionCommander.c ../codes/TS.c ../codes/analysis.c ../codes/Code_Assembly.c ../codes/parser.c
TestFiles= ParsTest.c
OutputName= test


.PHONY= all run code

all:
	$(CC) $(CFLAGS) $(CodeFiles) $(TestFiles) -o $(OutputName)


run: all
	./$(OutputName)


code: 
	$(CC) $(CFLAGS) $(CodeFiles) -o tmp_result


