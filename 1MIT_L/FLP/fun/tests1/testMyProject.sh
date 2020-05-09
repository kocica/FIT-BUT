#!/usr/bin/env bash

# Authors:
#    Pavel Hrebicek  (xhrebi04) - first 10 tests
#	   David Bolvansky (xbolva00) - 41 tests
#    Petr Stehlík    (xstehlXX) - 8 tests

# Test runner for FLP BKG-SIMPLIFY

RED='\033[0;31m'
GREEN='\033[0;32m'
YEL='\033[0;33m'
NC='\033[0m' # No Color
total=0
ok=0
TEST_COUNT=61

start=$(date +%s)

secs_to_human() {
    if [[ -z ${1} || ${1} -lt 60 ]] ;then
        min=0 ; secs="${1}"
    else
        time_mins=$(echo "scale=2; ${1}/60" | bc)
        min=$(echo ${time_mins} | cut -d'.' -f1)
        secs="0.$(echo ${time_mins} | cut -d'.' -f2)"
        secs=$(echo ${secs}*60|bc|awk '{print int($1+0.5)}')
    fi
    echo "Time Elapsed : ${min} minutes and ${secs} seconds."
}

echo "Starting testing..."
if [ ! -f "./simplify-bkg" ]
then
	echo -e "./simplify-bkg ${RED}not found.${NC}"
	exit 1
fi
echo "***********************************************"
for (( i=1; i<=TEST_COUNT; i++ ))
do
  ((total++))
  # -i
  ./simplify-bkg -i tests/test0${i}.in > tests/test0${i}_i.temp
  diff tests/test0${i}_i.temp tests/test0${i}_i.out  

  if [ "$?" == "0" ]
    then
      echo -e "[TEST 0$i] ./simplify-bkg -i tests/test0$i.in [${GREEN}OK${NC}]"
      ((ok++))
      rm tests/test0${i}_i.temp
    else
      echo -e "[TEST 0$i] ./simplify-bkg -i tests/test0$i.in [${RED}BAD${NC}]"
  fi

  ((total++))
  # -1
  ./simplify-bkg -1 tests/test0${i}.in > tests/test0${i}_1.temp
  diff tests/test0${i}_1.temp tests/test0${i}_1.out
  if [ "$?" == "0" ]
    then
      echo -e "[TEST 0$i] ./simplify-bkg -1 tests/test0${i}.in [${GREEN}OK${NC}]"
      ((ok++))
      rm tests/test0${i}_1.temp
    else
      echo -e "[TEST 0$i] ./simplify-bkg -1 tests/test0${i}.in [${RED}BAD${NC}]"
  fi

  ((total++))
   # -2
  ./simplify-bkg -2 tests/test0${i}.in > tests/test0${i}_2.temp
  diff tests/test0${i}_2.temp tests/test0${i}_2.out
  if [ "$?" == "0" ]
    then
      echo -e "[TEST 0$i] ./simplify-bkg -2 tests/test0${i}.in [${GREEN}OK${NC}]"
      ((ok++))
      rm tests/test0${i}_2.temp
    else
      echo -e "[TEST 0$i] ./simplify-bkg -2 tests/test0${i}.in [${RED}BAD${NC}]"
  fi
done

((total++))
./simplify-bkg -t
if [ "$?" == "1" ]
then
	((ok++))
	echo -e "./simplify-bkg -t [${GREEN}OK${NC}]"
else
	echo -e "./simplify-bkg -t [${RED}BAD${NC}]"
fi

((total++))
./simplify-bkg -i -1 -2
if [ "$?" == "1" ]
then
	((ok++))
	echo -e "./simplify-bkg -i -1 -2 [${GREEN}OK${NC}]"
else
	echo -e "./simplify-bkg -i -1 -2 [${RED}BAD${NC}]"
fi

((total++))
./simplify-bkg -i filenotexists
if [ "$?" == "1" ]
then
	((ok++))
	echo -e "./simplify-bkg -i filenotexists [${GREEN}OK${NC}]"
else
	echo -e "./simplify-bkg -i filenotexists [${RED}BAD${NC}]"
fi

((total++))
./simplify-bkg tests/test01.in 
if [ "$?" == "1" ]
then
	((ok++))
	echo -e "./simplify-bkg -i < tests/test01.in [${GREEN}OK${NC}]"
else
	echo -e "./simplify-bkg -i < tests/test01.in [${RED}BAD${NC}]"
fi

((total++))
./simplify-bkg
if [ "$?" == "1" ]
then
	((ok++))
	echo -e "./simplify-bkg [${GREEN}OK${NC}]"
else
	echo -e "./simplify-bkg [${RED}BAD${NC}]"
fi
		
echo "***********************************************"
echo -e "FINISHED TESTING:"
echo -e "OK/TOTAL: ${ok}/${total}"

if [ "$ok" == "$total" ]
  then
    echo -e "[${GREEN}SUCCESSFULL${NC}]"
   else
    echo -e "[${RED}FAILED${NC}]"
fi


