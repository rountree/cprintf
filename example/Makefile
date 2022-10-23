all: example.c
	clang -O0 -g -fsanitize=undefined,address -std=gnu2x -Wall -Wextra -Werror -I../include -o example example.c ../lib/libcprintf.a

clean:
	rm -f example
