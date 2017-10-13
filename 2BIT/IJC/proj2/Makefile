# Makefile
# IJC projekt 2
# Autor:     Filip Kocica, xkocic01 AT fit.vutbr.cz, FIT VUT
# Datum:     27.3.2017
# Prelozeno: gcc 5.4
# Popis: Zajisti preklad pouze zmenenych nebo neexistujicich souboru
#				 clean: smaze vsechny objektove a binarni soubory + knihovny.

# Typ prekladace pro C a pro C++
C=gcc
CC=g++

# Parametry prekladu # 1 pro c
CFLAGS=-std=c99 -pedantic -Wall -Wextra

# Parametry prekladu # 2 pro c++
CCFLAGS=-std=c++11 -pedantic -Wall

# Kde ma prekladac hledat dynamickou knihovnu
LD_LIBRARY_PATH=.

# vsechny objektove soubory pro knihovny
OF=hash_function.o htab_remove.o htab_init.o htab_clear.o htab_free.o htab_move.o htab_find.o htab_lookup_add.o htab_print.o htab_foreach.o htab_size.o htab_bucket_count.o

# Vytvori soubory
all: tail tail2 wordcount wordcount-dynamic

# 1) ==================================

# a)
tail: tail.o
	$(C) $(CFLAGS) tail.o -o $@

tail.o: tail.c tail.h
	$(C) $(CFLAGS) -c tail.c -o $@

# b)
tail2: tail2.o
	$(CC) $(CCFLAGS) tail2.o -o $@

tail2.o: tail2.cc
	$(CC) $(CCFLAGS) -c tail2.cc -o $@

# 2) ==================================

wordcount.o: wordcount.c htab.h
	$(C) $(CFLAGS) -c wordcount.c -o $@

io.o: io.c htab.h
	$(C) $(CFLAGS) -c io.c -o $@

hash_function.o: hash_function.c htab.h
	$(C) $(CFLAGS) -c hash_function.c -o $@

htab_print.o: htab_print.c htab.h
	$(C) $(CFLAGS) -c -fPIC htab_print.c -o $@

htab_foreach.o: htab_foreach.c htab.h
	$(C) $(CFLAGS) -c -fPIC htab_foreach.c -o $@

htab_remove.o: htab_remove.c htab.h
	$(C) $(CFLAGS) -c -fPIC htab_remove.c -o $@

htab_init.o: htab_init.c htab.h
	$(C) $(CFLAGS) -c -fPIC htab_init.c -o $@

htab_clear.o: htab_clear.c htab.h
	$(C) $(CFLAGS) -c -fPIC htab_clear.c -o $@

htab_free.o: htab_free.c htab.h
	$(C) $(CFLAGS) -c -fPIC htab_free.c -o $@

htab_move.o: htab_move.c htab.h
	$(C) $(CFLAGS) -c -fPIC htab_move.c -o $@

htab_find.o: htab_find.c htab.h
	$(C) $(CFLAGS) -c -fPIC htab_find.c -o $@

htab_lookup_add.o: htab_lookup_add.c htab.h
	$(C) $(CFLAGS) -c -fPIC htab_lookup_add.c -o $@

htab_size.o: htab_size.c htab.h
	$(C) $(CFLAGS) -c -fPIC htab_size.c -o $@

htab_bucket_count.o: htab_bucket_count.c htab.h
	$(C) $(CFLAGS) -c -fPIC htab_bucket_count.c -o $@

# staticka verze)
wordcount: wordcount.o io.o libhtab.a
	$(C) $(CFLAGS) wordcount.o io.o libhtab.a -o $@

libhtab.a: $(OF)
	ar rcs libhtab.a $(OF)

# dynamicka verze)
wordcount-dynamic: wordcount.o io.o libhtab.so
	$(C) $(CFLAGS) -L$(LD_LIBRARY_PATH) -Wl,-rpath=$(LD_LIBRARY_PATH) -fPIC wordcount.o io.o libhtab.so -o $@ libhtab.so

libhtab.so:
	$(C) -shared -o libhtab.so -fPIC $(OF) -L$(LD_LIBRARY_PATH)

# =====================================

# Vycisti adresar a necha jen zdrojove soubory
clean:
	rm -f *.o
	rm -f tail tail2 wordcount wordcount-dynamic libhtab.a libhtab.so

# Zabali pomoci programu zip
pack:
	zip xkocic01.zip *.c *.cc *.h Makefile
