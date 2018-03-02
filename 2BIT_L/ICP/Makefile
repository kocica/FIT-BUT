###############################################################
# @file    Makefile                                           #
# @author  Filip Kocica <xkocic01@stud.fit.vutbr.cz>          #
# @date    01/03/2018                                         #
#                                                             #
# ICP project Makefile                                        #
###############################################################

################## Build constants ##############
BIN_NAME   = BlockEditor

DOC        =
README     = README.txt

CC         = g++
CFLAGS     = -std=c++14
LDFLAGS    =
LIBS       =
DOXYGEN    = doxygen

SRC        = src

HEADERS    = $(wildcard $(SRC)/*.hpp)
SOURCES    = $(wildcard $(SRC)/*.cpp)
OBJS       = $(patsubst $(SRC)/%.cpp, $(SRC)/%.o, $(SOURCES))

################## Compilation ##################

all: $(BIN_NAME)

$(BIN_NAME): $(OBJS)
	$(CC) $(CFLAGS) $^ $(LDFLAGS) $(LIBS) -o $@

%.o: %.cpp %.hpp
	$(CXX) $(CFLAGS) -c $< -o $@

################## Pack/Clean ##################
	
.PHONY: clean

doxygen:
	$(DOXYGEN) $(SRC)/doxyConf

pack:
	zip -r xkocic01.zip Makefile $(README) ./src/* ./doc/* ./examples/*

clean:
	rm -rf $(BIN_NAME) $(SRC)/*.o

run:
	./BlockEditor