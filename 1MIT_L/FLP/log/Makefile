###############################################################################
# File: Makefile
# FLP - Rubiks cube
# Filip Kocica <xkocic01@stud.fit.vutbr.cz>
# 4/11/2020
###############################################################################

all:
	swipl -g main -q -o flp20-log -c flp20-log.pl

test:
	make clean 2>&1 1>/dev/null
	make 2>&1 1>/dev/null
	./flp20-log < in/in0
	./flp20-log < in/in1
	./flp20-log < in/in6
	./flp20-log < in/in7

test_performance:
	make clean 2>&1 1>/dev/null
	make 2>&1 1>/dev/null
	time ./flp20-log < in/in0
	time ./flp20-log < in/in1
	time ./flp20-log < in/in6
	time ./flp20-log < in/in7

clean:
	rm -f flp20-log