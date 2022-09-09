#include <stdio.h>
#include <assert.h>
#include <stdbool.h>
#include "cprintf.h"

int main(){
    /*
    cprintf("foo");
    cprintf("foo\n");
    cprintf("%s");
    cprintf("%#x");
    cprintf("%#30x");
    cprintf("%#030.7f");
    cprintf("%#030.7llf");
    */
    cprintf("%#030.7llf %s foo %I.8zu");
    return 0;

}

