# Makefile
# IJC projekt 1
# Autor:     Filip Kocica, xkocic01 AT fit.vutbr.cz, FIT VUT
# Datum:     1.3.2017
# Prelozeno: gcc 5.4
# Popis: Zajisti preklad pouze zmenenych nebo neexistujicich souboru
#				 clean: smaze vsechny .o a binarni soubory
#				 run: spusti primes a primes-i stylem "time ./program"
#							a steg-decode stylem "./steg-decode du1-obrazek.ppm"

# Typ prekladace
CC=gcc

# Parametry prekladu
CFLAGS=-O2 -std=c99 -pedantic -Wall

# Vytvori soubory
all: primes primes-i steg-decode steg-encode



primes: primes.o error.o eratosthenes.o
	$(CC) $(CFLAGS) primes.o error.o eratosthenes.o -lm -o $@

primes.o: bit_array.h primes.c eratosthenes.h
	$(CC) $(CFLAGS) -c primes.c -o $@

eratosthenes.o: eratosthenes.c eratosthenes.h bit_array.h
	$(CC) $(CFLAGS) -c eratosthenes.c -o $@



primes-i: primes-i.o error.o eratosthenes-i.o
	$(CC) $(CFLAGS) primes-i.o error.o eratosthenes-i.o -lm -o $@

primes-i.o: primes.c bit_array.h eratosthenes.c
	$(CC) $(CFLAGS) -DUSE_INLINE -c primes.c -o $@

eratosthenes-i.o: eratosthenes.c eratosthenes.h bit_array.h
	$(CC) $(CFLAGS) -DUSE_INLINE -c eratosthenes.c -o $@



steg-decode: steg-decode.o error.o ppm.o eratosthenes.o
	$(CC) $(CFLAGS) steg-decode.o error.o ppm.o eratosthenes.o -lm -o $@

steg-decode.o: steg-decode.c error.h ppm.h eratosthenes.h
	$(CC) $(CFLAGS) -c steg-decode.c -o $@



steg-encode: steg-encode.o error.o ppm.o eratosthenes.o
	$(CC) $(CFLAGS) steg-encode.o error.o ppm.o eratosthenes.o -lm -o $@

steg-encode.o: steg-encode.c error.h ppm.h eratosthenes.h
	$(CC) $(CFLAGS) -c steg-encode.c -o $@



ppm.o: ppm.c ppm.h error.h
	$(CC) $(CFLAGS) -c ppm.c -o $@

bit_array.h: error.o

error.o: error.c error.h
	$(CC) $(CFLAGS) -c error.c -o $@


clean:
	rm -f *.o
	rm -f primes primes-i steg-decode steg-encode

pack:
	zip xkocic01.zip *.c *.h Makefile

run:
	time ./primes
	time ./primes-i
	./steg-decode du1-obrazek.ppm
