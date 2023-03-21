CC = gcc
CFLAGS = -c -Wall -Werror -Wextra

all : s21_cat

s21_cat : s21_cat.o
	$(CC) s21_cat.o -o ./s21_cat
s21_cat.o : s21_cat.c
	$(CC) $(CFLAGS) s21_cat.c -o s21_cat.o

rebuild : clean
	make

clean : 
	rm *.o s21_cat

test :
	sh test_func_cat.sh

test_leak :
	sh test_leak_cat.sh

style :
	cp ../../materials/linters/.clang-format ./.clang-format
	clang-format -n ./*.c ./*.h
	