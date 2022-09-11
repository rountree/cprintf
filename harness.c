#include <stdio.h>
#include <assert.h>
#include <stdbool.h>
#include "cprintf.h"

int main(){
    cprintf("%d %d %d\n", 1, 2, 3);
    cprintf("%d %d %d\n", 10, 20, 30);
    cprintf("%d %d %d\n", 100, 200, 300);
    cflush();
    dump_graph_2();

    return 0;
}

