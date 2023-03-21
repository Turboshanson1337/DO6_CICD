CC = gcc
CFLAGS = -c -Wall -Werror -Wextra

all : s21_grep

s21_grep : s21_grep.o
	$(CC) s21_grep.o -o ./s21_grep
s21_grep.o : s21_grep.c
	$(CC) $(CFLAGS) s21_grep.c -o s21_grep.o

rebuild : clean
	make

clean :
	rm *.o s21_grep

test :
	sh test_func_grep.sh

test_leak :
	sh test_leak_grep.sh

test_leak_linux :
	bash test_ERROR_VALGRIND.sh

style :
	cp ../../materials/linters/.clang-format ./.clang-format
	clang-format -n ./*.c ./*.h
