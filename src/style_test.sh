#!/bin/bash

exit_code=0

cp style_output_check.sh cat/
cp style_output_check.sh grep/

cd cat/
clang-format -n --style=Google *.c *.h 2> clang_output.txt
sudo chmod +x style_output_check.sh
echo s21_cat:
sh style_output_check.sh
if [ $? -ne 0 ]; then
  exit_code=1
fi

cd ../grep/
clang-format -n --style=Google *.c *.h 2> clang_output.txt
sudo chmod +x style_output_check.sh
echo s21_grep:
sh style_output_check.sh
if [ $? -ne 0 ]; then
  exit_code=1
fi

exit $exit_code