// Copyright 2022 Lawrence Livermore National Security, LLC and other
// libjustify Project Developers. See the top-level LICENSE file for details.
//
// SPDX-License-Identifier: MIT

#include <stdio.h>
#include <assert.h>
#include <stdbool.h>
#include "cprintf.h"

int main(){
    printf("fprintf():\n");
    fprintf( stderr, "%d %d %d\n", 1, 2, 3);
    fprintf( stderr, "%d %d %d\n", 10, 20, 30);
    fprintf( stderr, "%d %d %d\n", 100, 200, 300);
    printf("\n");

    printf("cfprintf():\n");
    cfprintf( stderr, "%d %d %d\n", 1, 2, 3);
    cfprintf( stderr, "%d %d %d\n", 10, 20, 30);
    cfprintf( stderr, "%d %d %d\n", 100, 200, 300);
    cflush();

    printf("\n\n============\n\n");

    printf("printf():\n");
    printf("a=%07.4f b= %07.5Lf\n", 1.2, 1.2L );
    printf("a=%07.4lf b= %07.5Lf\n", 10.22, 10.22L );
    printf("a=%07.4lf b= %07.5Lf\n", 100.222, 100.222L );
    printf("a=%07.4lf b= %07.5Lf\n", 1000.2222, 1000.2222L );
    printf("\n");

    printf("cprintf():\n");
    cprintf("a=%07.4f b= %07.5Lf\n", 1.2, 1.2L );
    cprintf("a=%07.4lf b= %07.5Lf\n", 10.22, 10.22L );
    cprintf("a=%07.4lf b= %07.5Lf\n", 100.222, 100.222L );
    cprintf("a=%07.4lf b= %07.5Lf\n", 1000.2222, 1000.2222L );
    cflush();

    printf("\n\n============\n\n");

    printf("printf():\n");
    printf("%s | %s | %s | %-s \n", "tiny", "longer", "pretty long", "downright wordy");
    printf("%0d | %.2f | %p | %c \n", 0, 3.14, main, 'w');
    printf("%0d | %.2f | %p | %-c \n", 0, 3.14, main, 'x');
    printf("%0d | %.2f | %p | %c \n", 20000, 300000.14, cprintf, 'y');
    printf("%0d | %.2f | %p | %-c \n", 20, 30.14, printf, 'z');
    printf("\n");

    printf("cprintf():\n");
    cprintf("%s | %s | %s | %-s \n", "tiny", "longer", "pretty long", "downright wordy");
    cprintf("%0d | %.2f | %p | %c \n", 0, 3.14, main, 'x');
    cprintf("%0d | %.2f | %p | %-c \n", 0, 3.14, main, 'x');
    cprintf("%0d | %.2f | %p | %c \n", 20000, 300000.14, cprintf, 'y');
    cprintf("%0d | %.2f | %p | %-c \n", 20, 30.14, printf, 'z');
    cflush();

    return 0;
}

