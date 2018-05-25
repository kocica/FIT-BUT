# @file    Makefile
# @author  Filip Kocica <xkocic01@stud.fit.vutbr.cz>
# @date    28/2/2018
#
# IPK project 1
#
# Client <-> Server communication
# Grab data about users and display them.


################## Build constants ##################
SERVER_BIN = ipk-server
CLIENT_BIN = ipk-client
BASE64_BIN = base64

DOC        = dokumentace.pdf
README     = README.txt

CC         = g++
CFLAGS     = -std=c++11
LDLIBS     = -static-libstdc++

################## Source names ##################
SERVER     = ipk-server.cpp
CLIENT     = ipk-client.cpp
BASE_HPP   = base64.hpp
BASE_CPP   = base64.cpp
################## Path to source ##################
SRC        = .

all: $(CLIENT_BIN) $(SERVER_BIN)

################## base64 build ##################
$(BASE64_BIN).o: $(BASE_HPP) $(BASE_CPP)
	$(CC) $(CFLAGS) -c $(SRC)/$(BASE_CPP) -o $(SRC)/$@

################## Client build ##################
$(CLIENT_BIN): $(CLIENT_BIN).o $(BASE64_BIN).o
	$(CC) $(CFLAGS) $(CLIENT_BIN).o $(LDLIBS) $(BASE64_BIN).o -o $(SRC)/$@
	
$(CLIENT_BIN).o: $(CLIENT)
	$(CC) $(CFLAGS) -c $(SRC)/$(CLIENT) $(LDLIBS) -o $(SRC)/$@
	
################## Server build ##################
$(SERVER_BIN): $(SERVER_BIN).o $(BASE64_BIN).o
	$(CC) $(CFLAGS) $(SERVER_BIN).o $(LDLIBS) $(BASE64_BIN).o -o $(SRC)/$@
	
$(SERVER_BIN).o: $(SERVER)
	$(CC) $(CFLAGS) -c $(SRC)/$(SERVER) $(LDLIBS) -o $(SRC)/$@

	
.PHONY: clean

################## Pack/Clean ##################
pack:
	zip -r xkocic01.zip Makefile $(CLIENT) $(SERVER) $(DOC) $(README) $(BASE_CPP) $(BASE_HPP)

clean:
	rm -rf $(CLIENT_BIN) $(SERVER_BIN) *.o