lib: cprintf.c cprintf.h
	clang -O0 -g -fsanitize=undefined,address -std=gnu2x -Wall -Wextra -Werror -c -fPIC cprintf.c
	ar r libcprintf.a cprintf.o
	clang -O0 -g -std=gnu2x -shared -o libcprintf.so cprintf.o

test: lib harness.c
	clang -O0 -g -fsanitize=undefined,address -std=gnu2x -Wall -Wextra -Werror -o harness harness.c libcprintf.a
	./harness

clean:
	rm -f *.o *.so *.a harness
