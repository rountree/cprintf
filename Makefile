CC=clang-14
all: src/cprintf.c include/cprintf.h
	mkdir -p ./lib
	$(CC) -O0 -g -std=gnu2x -Wall -Wextra -Werror -c -fPIC -I./include src/cprintf.c
	ar r ./lib/libcprintf.a cprintf.o
	$(CC) -O0 -g -std=gnu2x -shared -o ./lib/libcprintf.so cprintf.o

clean:
	rm -f cprintf.o ./lib/libcprintf.so ./lib/libcprintf.a
