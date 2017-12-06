# ============================================================
# Makefile                                                   *
# IOS projekt 2                                              *
# Autor:     Filip Kocica, xkocic01 AT fit.vutbr.cz, FIT VUT *
# Datum:     30.3.2017                                       *
# Prelozeno: gcc 5.4                                         *
# ============================================================

# Typ prekladace
CC=gcc
NAME=proj2

# Parametry prekladu
CFLAGS=-std=gnu99 -Wall -Wextra -Werror -pedantic
LFLAGS=-lpthread

OBJ_F=$(NAME).o

all: $(NAME)

# Vytvor spustitelny soubor
$(NAME): $(OBJ_F) $(NAME).h
	$(CC) $(CFLAGS) $(OBJ_F) -o $@ $(LFLAGS)

# Vytvor objektovy soubor
$(NAME).o: $(NAME).c $(NAME).h
	$(CC) $(CFLAGS) -c $(NAME).c -o $@ $(LFLAGS)

# Uklid
clean:
	rm -f *.o $(NAME)

# Zabal
pack:
	zip $(NAME).zip *.c *.h Makefile
