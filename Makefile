CC=gcc
CFLAGS=-Wall -Wextra -Werror -std=c99
CodeFiles= #sem napis, ktore file-y chces prelozit !aj adresu, ak je inde
TestFiles= #sem napis testovacie file-y	!aj s adresou, ak su v inom subore napr. ../codes/test.c
OutputName= #sem napis vysledne meno


.PHONY= all run code

all:
	$(CC) $(CFLAGS) $(CodeFiles) $(TestFiles) -o $(OutputName)


run: all
	./$(OutputName)


code: 
	$(CC) $(CFLAGS) $(CodeFiles) -o tmp_result


