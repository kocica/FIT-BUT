# @file    Makefile
# @author  Filip Kocica <xkocic01@stud.fit.vutbr.cz>
# @date    25/09/2018
#
# ISA project -- Feed reader in Atom format with TLS support


################## Build constants ##################
BIN        = feedreader

DOC        = manual.pdf
README     = README
TEST       = test

CC         = g++
CFLAGS     = -std=c++14
LDLIBS     = -static-libstdc++

XMLFLAGS   = `xml2-config --cflags`
XMLLIBS    = `xml2-config --libs`

#SSLFLAGS   = `pkg-config --cflags openssl`
SSLLIBS    = -lcrypto -lssl

# wildcard doesnt work on eva server :(
HEADERS    = Arguments.h Feed.h FeedreaderException.h SSL.h Utils.h XMLManager.h
SOURCES    = Arguments.cpp Feed.cpp main.cpp SSL.cpp Utils.cpp XMLManager.cpp
OBJS       = Arguments.o Feed.o main.o SSL.o Utils.o XMLManager.o

#HEADERS    = $(wildcard *.h)
#SOURCES    = $(wildcard *.cpp)
#OBJS       = $(patsubst %.cpp, %.o, $(SOURCES))

SRC        = .


################## Compilation  #####################

all: $(BIN)

$(BIN): $(HEADERS) $(SOURCES) $(OBJS)
	$(CC) $(CFLAGS) $(XMLFLAGS) $(OBJS) -o $@  $(SSLLIBS) $(XMLLIBS) $(LDLIBS)

Arguments.o: Arguments.cpp
	$(CC) $(CFLAGS) $(XMLFLAGS)  -c $< -o $@  $(SSLLIBS) $(XMLLIBS) $(LDLIBS)

Feed.o: Feed.cpp
	$(CC) $(CFLAGS) $(XMLFLAGS)  -c $< -o $@  $(SSLLIBS) $(XMLLIBS) $(LDLIBS)

SSL.o: SSL.cpp
	$(CC) $(CFLAGS) $(XMLFLAGS)  -c $< -o $@  $(SSLLIBS) $(XMLLIBS) $(LDLIBS)

Utils.o: Utils.cpp
	$(CC) $(CFLAGS) $(XMLFLAGS)  -c $< -o $@  $(SSLLIBS) $(XMLLIBS) $(LDLIBS)

XMLManager.o: XMLManager.cpp
	$(CC) $(CFLAGS) $(XMLFLAGS)  -c $< -o $@  $(SSLLIBS) $(XMLLIBS) $(LDLIBS)
	
main.o: main.cpp
	$(CC) $(CFLAGS) $(XMLFLAGS)  -c $< -o $@  $(SSLLIBS) $(XMLLIBS) $(LDLIBS)

################## Pack, Clean & Test ###############

.PHONY: test clean

test:
	chmod +x ./test/testing_script.sh
	./test/testing_script.sh

pack:
	tar -cf xkocic01.tar *.cpp *.h Makefile $(README) $(DOC) $(TEST)

clean:
	rm -f $(BIN) *.o