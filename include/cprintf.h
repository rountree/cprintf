// Copyright 2022 Lawrence Livermore National Security, LLC and other
// libjustify Project Developers. See the top-level LICENSE file for details.
//
// SPDX-License-Identifier: MIT

#include <stdio.h>
#include <stdarg.h>
#ifndef __CPRINTF_H_HEADER
#define __CPRINTF_H_HEADER
void cprintf( const char* fmt, ... );
void cfprintf( FILE *stream, const char* fmt, ... );

void cvprintf( const char* fmt, va_list args );
void cvfprintf( FILE *stream, const char* fmt, va_list args );

void cflush( void );
#endif


