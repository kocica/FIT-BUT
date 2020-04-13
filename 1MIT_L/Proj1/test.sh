#!/bin/bash

# ----------------------------------------------------------------------------
# Author : Filip Kocica <xkocic01@stud.fit.vutbr.cz>
# Brief  : PRL - Project 2 - Odd-even transposition sort
# Date   : 08.03.2020
# File   : test.sh
# ----------------------------------------------------------------------------

# ----------------------------------------------------------------------------
# Constants
BINARY_NAME="ots"
SOURCE_NAME="ots.cpp"
OUTPUT_NAME="numbers"

# ----------------------------------------------------------------------------
# 1) Get input number or exit
if [ $# -eq 1 ]
    then
        input=$1;
else
    echo "Error: In-correct input."
    exit 1
fi

# ----------------------------------------------------------------------------
# 2) Compile source code
mpic++ --prefix /usr/local/share/OpenMPI -o $BINARY_NAME $SOURCE_NAME

# ----------------------------------------------------------------------------
# 3) Create file with random numbers separated by space
# bs    = BYTES - read and write up to bytes
# if    = FILE  - input file
# of    = FILE  - output file
# count = N     - only N input blocks
# Note: Output redirected to null not to show garbage.
dd bs=1 if=/dev/random of=$OUTPUT_NAME count=$input 2>/dev/null

# ----------------------------------------------------------------------------
# 4) Execute odd-even transposition sort
mpirun --prefix /usr/local/share/OpenMPI -np $input $BINARY_NAME

# ----------------------------------------------------------------------------
# 5) Cleanup
rm -rf $BINARY_NAME $OUTPUT_NAME
