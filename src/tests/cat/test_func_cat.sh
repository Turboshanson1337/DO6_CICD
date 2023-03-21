#!/bin/bash

SUCCESS=0
FAIL=0
COUNTER=0
DIFF_RES=""
GREEN='\033[32m'
RED='\033[31m'
NORMAL='\033[0m'

declare -a tests=(
"VAR test_case_cat.txt"
)

declare -a extra=(
"-s test_1_cat.txt"
"-b -e -n -s -t -v test_1_cat.txt"
"-t test_3_cat.txt"
"-n test_2_cat.txt"
"-n -b test_1_cat.txt"
"-s -n -e test_4_cat.txt"
"-n test_1_cat.txt"
"-n test_1_cat.txt test_2_cat.txt"
"-v test_5_cat.txt"
)

testing()
{
    t=$(echo $@ | sed "s/VAR/$var/")
    ./s21_cat $t &> test_s21_cat.log
    cat $t &> test_sys_cat.log
    DIFF_RES="$(diff -s test_s21_cat.log test_sys_cat.log)"
    (( COUNTER++ ))
    if [ "$DIFF_RES" == "Files test_s21_cat.log and test_sys_cat.log are identical" ]
    then
      (( SUCCESS++ ))
      echo -e "Test #$COUNTER ${GREEN}OK${NORMAL}"
    else
      (( FAIL++ ))
      echo -e "Test #$COUNTER ${RED}FAIL${NORMAL}"
    fi
    rm test_s21_cat.log test_sys_cat.log
}

# специфические тесты
for i in "${extra[@]}"
do
    var="-"
    testing $i
done

# 1 параметр
for var1 in b e n s t v
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