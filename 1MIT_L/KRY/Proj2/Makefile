# @file    Makefile
# @author  Filip Kocica <xkocic01@stud.fit.vutbr.cz>
# @date    22/03/2020
#
# KRY project 2 -- RSA Implementation

################## Build constants ##################
BIN        = kry

CC         = g++
CFLAGS     = -std=c++14
LDLIBS     = -static-libstdc++

SSLLIBS    = -lgmp

HEADERS    = $(wildcard *.h)
SOURCES    = $(wildcard *.cpp)
OBJS       = $(patsubst %.cpp, %.o, $(SOURCES))

SRC        = .

################## Compilation  #####################

all: $(BIN)

$(BIN): $(HEADERS) $(SOURCES) $(OBJS)
	$(CC) $(CFLAGS) $(XMLFLAGS) $(OBJS) -o $@  $(SSLLIBS) $(XMLLIBS) $(LDLIBS)

%.o: %.cpp %.hpp
	$(CC) $(CFLAGS) -c $< -o $@

################## Clean #####################

.PHONY: clean

clean:
	rm -f $(BIN) *.o