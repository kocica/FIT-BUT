# Makefile
# IFJ; IAL project
# Author     :     Filip Kocica, xkocic01 AT fit.vutbr.cz, FIT VUT
# Date       :     24.9.2017
# Compiled   :     gcc 5.4
# Description:     Compiles source codes with flags CFLAGS and link
#                  together to one binary program
#	Clean      :     Removes object files

# Compilator type
CC=gcc

# Parameters of compilation
CFLAGS=-std=c11 -pedantic -Wall -Wextra -Werror -DSCANNER_DEBUG #-DPARSER_DEBUG

OBJS=main.o scanner.o parser.o reserved.o symtable.o error.o string.o embedded_funcs.o stack.o

SRC=src

# Name of project
PROG=ifj_compiler

# All rule
all: $(PROG)

$(PROG): $(OBJS) $(SRC)/token.h $(SRC)/types.h
	$(CC) $(CFLAGS) $(OBJS) -o $@

stack.o: $(SRC)/stack.c $(SRC)/stack.h
	$(CC) $(CFLAGS) -c $(SRC)/stack.c -o $@

scanner.o: $(SRC)/scanner.c $(SRC)/scanner.h
	$(CC) $(CFLAGS) -c $(SRC)/scanner.c -o $@

parser.o: $(SRC)/parser.c $(SRC)/parser.h
	$(CC) $(CFLAGS) -c $(SRC)/parser.c -o $@

reserved.o: $(SRC)/reserved.c $(SRC)/reserved.h
	$(CC) $(CFLAGS) -c $(SRC)/reserved.c -o $@

symtable.o: $(SRC)/symtable.c $(SRC)/symtable.h
	$(CC) $(CFLAGS) -c $(SRC)/symtable.c -o $@

string.o: $(SRC)/string.c $(SRC)/string.h
	$(CC) $(CFLAGS) -c $(SRC)/string.c -o $@

error.o: $(SRC)/error.c $(SRC)/error.h
	$(CC) $(CFLAGS) -c $(SRC)/error.c -o $@

embedded_funcs.o: $(SRC)/embedded_funcs.c $(SRC)/embedded_funcs.h
	$(CC) $(CFLAGS) -c $(SRC)/embedded_funcs.c -o $@

main.o: $(SRC)/main.c
	$(CC) $(CFLAGS) -c $(SRC)/main.c -o $@

.PHONY: clean

run:
	./ifj_compiler < INPUT_TEST.txt

pack:
	zip -r xkocic01.zip .

clean:
	rm -rf *.o
	rm -rf $(PROG)
