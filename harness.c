#include <stdio.h>
#include <assert.h>
#include <stdbool.h>
#include "cprintf.h"

int main(){
    cprintf("%d %d %d\n", 1, 2, 3);
    cprintf("%d %d %d\n", 10, 20, 30);
    cprintf("%d %d %d\n", 100, 200, 300);
    cflush();
    free_graph();

    printf("============\n");

    cprintf("a=%07.4f b= %07.5Lf\n", 1.2, 1.2L );
    cprintf("a=%07.4lf b= %07.5Lf\n", 10.22, 10.22L );
    cprintf("a=%07.4lf b= %07.5Lf\n", 100.222, 100.222L );
    cprintf("a=%07.4lf b= %07.5Lf\n", 1000.2222, 1000.2222L );
    cflush();
    free_graph();

    printf("============\n");

    cprintf("%s | %s | %s | %-s \n", "tiny", "longer", "pretty long", "downright wordy");
    cprintf("%0d | %.2f | %p | %-c \n", 0, 3.14, main, 'x');
    cprintf("%0d | %.2f | %p | %-c \n", 20000, 300000.14, cprintf, 'y');
    cprintf("%0d | %.2f | %p | %-c \n", 20, 30.14, printf, 'z');
    cflush();
    free_graph();

    return 0;
}

