% CPRINTF(3) Version 0.0 | cprintf, cfprintf, csnprintf, cvfprintf, cvsprintf, cvsnprintf, cflush
% Barry Rountree (rountree@llnl.gov)

NAME
====
cprintf and friends - automatically formatted table output

SYNOPSIS
========
#include <cprintf.h>

-lcprintf

int cprintf(const char *format, ...);

int cfprintf(FILE *stream, const char *format, ...);

void* cflush();

DESCRIPTION
===========


