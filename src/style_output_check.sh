#!/bin/bash

if [ -f "clang_output.txt" ];
then
	if [ -s "clang_output.txt" ];
	then
		clang-format -n --style=Google *.c *.h
		exit 1
	else
		echo "Style test OK"
	fi
else
	echo "Missing file clang_output.txt \n Run style test first"
	exit 1
fi
