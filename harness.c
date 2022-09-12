#include <stdio.h>
#include <assert.h>
#include <stdbool.h>
#include "cprintf.h"

int main(){
    cprintf("%d %d %d\n", 1, 2, 3);
    cprintf("%d %d %d\n", 10, 20, 30);
    cprintf("%d %d %d\n", 100, 200, 300);
    cflush();
    dump_graph();
    free_graph();

    printf("============\n");
    fflush(NULL);

    cprintf("a=%07.4f b= %07.5Lf\n", 1.2, 1.2L );
    /*
    cprintf("a=%07.4lf b= %07.5lf\n", 10.22, 10.22L );
    cprintf("a=%07.4lf b= %07.5lf\n", 100.222, 100.222L );
    cprintf("a=%07.4lf b= %07.5lf\n", 1000.2222, 1000.2222L );
    */
    cflush();
    dump_graph();
    free_graph();
    return 0;
}

