# @file    Makefile
# @author  Filip Kocica <xkocic01@stud.fit.vutbr.cz>
# @date    7/3/2018
#
# IPK project 2
#
# Bandwidth Measurement
#   Find out maximal, minimal and regular speeds of end-to-end
#   connections via UDP protocol.


################## Build constants ##################
TARGET     = ipk-mtrip

DOC        = dokumentace.pdf
README     = README.txt

CC         = g++
CFLAGS     = -std=c++11
LDLIBS     = -pthread -static-libstdc++ -lm

################## Source names ##################
TARGET_CPP = ipk-mtrip.cpp

################## Path to source ##################
SRC        = .

all: $(TARGET)
	
################## Target build ##################
$(TARGET): $(TARGET).o
	$(CC) $(CFLAGS) $(TARGET).o $(LDLIBS) -o $(SRC)/$@

$(TARGET).o: $(SRC)/$(TARGET_CPP)
	$(CC) $(CFLAGS) -c $(SRC)/$(TARGET_CPP) $(LDLIBS) -o $@

################## Pack/Clean ##################

.PHONY: clean

pack:
	zip -r xkocic01.zip Makefile $(TARGET_CPP) $(DOC) $(README)

clean:
	rm -rf $(TARGET) *.o