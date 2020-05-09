#!/bin/bash

# ----------------------------------------------------------------------------
# Author : Filip Kocica <xkocic01@stud.fit.vutbr.cz>
# Brief  : PRL - Project 3 - Visibility
# Date   : 21.03.2020
# File   : test.sh
# ----------------------------------------------------------------------------

# ----------------------------------------------------------------------------
# Constants
BINARY_NAME="vid"
SOURCE_NAME="vid.cpp"
NP=1

# ----------------------------------------------------------------------------
# 1) Get input string or exit
if [ $# -eq 1 ];
then
    input=$1;
else
    echo "Error: In-correct input."
    exit 1
fi

# ----------------------------------------------------------------------------
# 2) Compute number of processors
number_count=0

for i in $(echo $input | tr "," "\n");
do
    ((number_count++))
done

#((number_count--))

for i in $(seq 0 10);
do
    power=$(echo "2^$i" | bc)

    if [ $power -gt $number_count ];
    then
        NP=$(echo "2^($i-1)" | bc)
        break;
    fi
done

# ----------------------------------------------------------------------------
# 3) Compile source code
mpic++ --prefix /usr/local/share/OpenMPI -o $BINARY_NAME $SOURCE_NAME

# ----------------------------------------------------------------------------
# 4) Execute visibility binary
mpirun --prefix /usr/local/share/OpenMPI -np $NP $BINARY_NAME $input

# ----------------------------------------------------------------------------
# 5) Cleanup
rm -rf $BINARY_NAME
