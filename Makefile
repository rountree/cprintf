all: src/cprintf.c include/cprintf.h
	mkdir -p ./lib
	clang -O0 -g -std=gnu2x -Wall -Wextra -Werror -c -fPIC -I./include src/cprintf.c
	ar r ./lib/libcprintf.a cprintf.o
	clang -O0 -g -fsanitize=undefined,address -std=gnu2x -shared -o ./lib/libcprintf.so cprintf.o

clean:
	rm -f cprintf.o ./lib/libcprintf.so ./lib/libcprintf.a
