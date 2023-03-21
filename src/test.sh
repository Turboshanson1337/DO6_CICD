#!/bin/bash

exit_code=0
source_dir_cat="./tests/cat"
source_dir_grep="./tests/grep"
target_dir_cat="./cat"
target_dir_grep="./grep"
files_cat=("test_1_cat.txt" "test_2_cat.txt" "test_3_cat.txt" "test_4_cat.txt" "test_5_cat.txt" "test_case_cat.txt" "test_func_cat.sh")
files_grep=("test_0_grep.txt" "test_1_grep.txt" "test_2_grep.txt" "test_3_grep.txt" "test_4_grep.txt" "test_5_grep.txt" "test_6_grep.txt" "test_ptrn_grep.txt" "test_func_grep.sh")

for file in "${files_cat[@]}"
do
  cp "$source_dir_cat/$file" "$target_dir_cat"
done

for file in "${files_grep[@]}"
do
  cp "$source_dir_grep/$file" "$target_dir_grep"
done

sudo chmod +x ./cat/test_func_cat.sh ./grep/test_func_grep.sh

cd cat/
./test_func_cat.sh
if [ $? -ne 0 ]; then
  exit_code=1
fi

cd ../grep/
./test_func_grep.sh
if [ $? -ne 0 ]; then
  exit_code=1
fi

exit $exit_code