#!/bin/bash

SUCCESS=0
FAIL=0
COUNTER=0
DIFF_RES=""
GREEN='\033[32m'
RED='\033[31m'
NORMAL='\033[0m'

declare -a tests=(
"s test_0_grep.txt VAR"
"for s21_grep.c s21_grep.h Makefile VAR"
"for s21_grep.c VAR"
)

declare -a extra=(
"-ce ^int test_1_grep.txt test_2_grep.txt"
"-e ^int test_1_grep.txt"
"-nivh = test_1_grep.txt test_2_grep.txt"
)

testing()
{
    t=$(echo $@ | sed "s/VAR/$var/")
    ./s21_grep $t &> test_s21_grep.log
    grep $t &> test_sys_grep.log
    DIFF_RES="$(diff -s test_s21_grep.log test_sys_grep.log)"
    (( COUNTER++ ))
    if [ "$DIFF_RES" == "Files test_s21_grep.log and test_sys_grep.log are identical" ]
    then
      (( SUCCESS++ ))
      echo -e "Test #$COUNTER ${GREEN}OK${NORMAL}"
    else
      (( FAIL++ ))
      echo -e "Test #$COUNTER ${RED}FAIL${NORMAL}"
    fi
    rm test_s21_grep.log test_sys_grep.log
}

# специфические тесты
for i in "${extra[@]}"
do
    var="-"
    testing $i
done

# 1 параметр
for var1 in v c n h
do
    for i in "${tests[@]}"
    do
        var="-$var1"
        testing $i
    done
done

if [ $FAIL -ne 0 ];
then
    echo "Functional tests: FAIL"
    exit 1
else
    echo "Functional tests: OK"
fi