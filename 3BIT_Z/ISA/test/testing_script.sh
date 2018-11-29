#!/bin/sh
# Testing ISA project
# Filip Kocica <xkocic01@stud.fit.vutbr.cz>

FAULTS=0
OK=0
TESTS=7

echo
echo "-----------------------------"
echo "|  BASIC ISA PROJECT TESTS  |"
echo "-----------------------------"
echo

if [ ! -f ./feedreader ]; then
    echo "INFO: Binary file not found - compiling."
    echo
    make 2>/dev/null # Compilation
    echo
    echo "INFO: Compilation done. Running tests"
    echo
    echo
fi

# TEST 1 Args error check
./feedreader 2>/dev/null

if [ $? != 0 ]; then
    echo "-> TEST 1        [OK]"
    OK=$((OK+1));
else
    echo "-> TEST 1        [FAIL]"
    FAULTS=$((FAULTS+1));
fi

# TEST 2 Connection error check
./feedreader http://www.notexisting42.com/what42 2>./test/ProjResults/02.out

diff ./test/ProjResults/02.out ./test/RefResults/02.ref > ./test/ProjResults/02.err 2>/dev/null

if [ $? != 0 ]; then
    echo "-> TEST 2        [OK]"
    OK=$((OK+1));
else
    echo "-> TEST 2        [FAIL]"
    FAULTS=$((FAULTS+1));
fi

# TEST 3 Validation error check
./feedreader http://www.fit.vutbr.cz:443/news/news-rss.php 2>./test/ProjResults/03.out

diff ./test/ProjResults/03.out ./test/RefResults/03.ref > ./test/ProjResults/03.err 2>/dev/null

if [ $? != 0 ]; then
    echo "-> TEST 3        [OK]"
    OK=$((OK+1));
else
    echo "-> TEST 3        [FAIL]"
    FAULTS=$((FAULTS+1));
fi

# TEST 4 Wrong certificate error
./feedreader https://what-if.xkcd.com/feed.atom -C /dev/null 2>./test/ProjResults/04.out

diff ./test/ProjResults/04.out ./test/RefResults/04.ref > ./test/ProjResults/04.err 2>/dev/null

if [ $? != 0 ]; then
    echo "-> TEST 4        [OK]"
    OK=$((OK+1));
else
    echo "-> TEST 4        [FAIL]"
    FAULTS=$((FAULTS+1));
fi

# TEST 5 Args error check
./feedreader -u -u 2>/dev/null

if [ $? != 0 ]; then
    echo "-> TEST 5        [OK]"
    OK=$((OK+1));
else
    echo "-> TEST 5        [FAIL]"
    FAULTS=$((FAULTS+1));
fi

# TEST 6 FIT news check
./feedreader https://what-if.xkcd.com/feed.atom > ./test/ProjResults/06.out 2>/dev/null

diff ./test/ProjResults/06.out ./test/RefResults/06.ref > ./test/ProjResults/06.err 2>/dev/null

if [ $? == 0 ]; then
    echo "-> TEST 6        [OK]"
    OK=$((OK+1));
else
    echo "-> TEST 6        [FAIL]"
    FAULTS=$((FAULTS+1));
fi

# TEST 7 Feedfile test
./feedreader -f ./test/feedfile > ./test/ProjResults/07.out 2>/dev/null

diff ./test/ProjResults/07.out ./test/RefResults/07.ref > ./test/ProjResults/07.err 2>/dev/null

if [ $? == 0 ]; then
    echo "-> TEST 7        [OK]"
    OK=$((OK+1));
else
    echo "-> TEST 7        [FAIL]"
    FAULTS=$((FAULTS+1));
fi


echo
echo "---------------------------"
echo "| Number of tests : $TESTS     |"
echo "| OK              : $OK     |"
echo "| FAIL            : $FAULTS     |"
echo "---------------------------"

echo
echo "The feeds _may_ be different from the ones in ref folder"
echo "and most likely some tests will fail even though program is correct."
echo