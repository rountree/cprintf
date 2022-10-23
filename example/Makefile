all: harness.c
	clang -O0 -g -fsanitize=undefined,address -std=gnu2x -Wall -Wextra -Werror -I../include -o harness harness.c ../lib/libcprintf.a
	./harness

clean:
	rm -f harness
