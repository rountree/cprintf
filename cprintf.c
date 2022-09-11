#include <stdbool.h>    // true and false
#include <stddef.h>     // NULL
#include <assert.h>     // assert
#include <string.h>     // strncmp
#include <stdlib.h>     // calloc
#include <stdio.h>      // fprintf
#include <string.h>     // strspan
#include <stdarg.h>     // variadic
#include <wchar.h>      // wint_t
#include <uchar.h>
#include <stdint.h>     // intmax_t
#include "cprintf.h"

static struct atom * origin = NULL;



void
dump_atom( struct atom * a ){
    fprintf(stdout, "%s:%d orig=%6s w=%2zu a=%14p a->up=%14p a->down=%14p a->left=%14p a->right=%14p\n",
            __FILE__, __LINE__, a->original_specification, a->original_field_width, 
            a, a->up, a->down, a->left, a->right );
}

void
dump_graph( void ){
    struct atom *a, *linestart=origin;
    static size_t line=0;
    while(linestart != NULL){
        fprintf(stdout, "%s:%d Line %zu\n", __FILE__, __LINE__, line++ );
        a = linestart;
        while( a != NULL ){
            dump_atom( a );
            a = a->right;
        }
        linestart = linestart->down;
    }
}

void
free_graph( struct atom *a ){
    if( NULL == origin ){
        return;
    }
    if( NULL == a ){
        a = origin;
    }
    if( a->right ){
        free_graph( a->right );
    }
    if( a->down ){
        free_graph( a->down );
    }
    if( a == origin ){
        origin = NULL;
    }
    if( a->up ){
        a->up->down = NULL;
    }
    if( a->left ){
        a->left->right = NULL;
    }
    free( a );
    return;
}


struct atom *
create_atom( bool is_newline ){
    /* "Creating" an atom means grabbing the next unpopulated atom and creating
     * new atoms below and to the right of it, then updating the links.*/
    static struct atom *last_populated_atom = NULL;
    static struct atom *last_populated_line = NULL;

    struct atom *a;

    if( NULL == origin ){
        origin = calloc( sizeof( struct atom ), 1 );
        a = last_populated_atom = last_populated_line = origin;
        a->populated = true;
    }else if(is_newline){
        a = last_populated_line;
        a->down = calloc( sizeof( struct atom ), 1 );
        assert( a->down );
        a->down->up = a;
        a->down->populated = true;
        last_populated_atom = last_populated_line = a->down;
    }else{
        a = last_populated_atom;
        a->right = calloc( sizeof( struct atom ), 1 );
        assert( a->right );
        a->right->left = a;
        if( a->up ){
            a->right->up = a->up->right;
            a->up->right->down = a->right;
        }
        a->right->populated = true;
        last_populated_atom = a->right;
    }
    return last_populated_atom;
}


// Conversion specifications look like this:
// %[flags][field_width][.precision][length_modifier]specifier


ptrdiff_t
parse_flags( const char *p ){
    // Returns the number of bytes starting from the beginning
    // of p that consist only of the characters #0- +'I
    return strspn( p, "#0- +'I" );
}

ptrdiff_t
parse_field_width( const char *p ){
    // Returns the number of bytes in the field width,
    // or zero of no field width specified.
    char *end;
    assert( '*' != *p );    // Don't support * or *n$ (yet).
    strtol( p, &end, 10 );
    return end - p;
}

 ptrdiff_t
parse_precision( const char *p ){
    char *end;
    // Returns the number of bytes representing
    // precision, including the leading '.'.  
    if( '.' == *p ){
        assert( '*' != *(p+1) ); // Don't support * or *n$ (yet).
        strtol( p+1, &end, 10 );
        return (end - p); 
    }else{
        return 0;
    }
}

ptrdiff_t 
parse_length_modifier( const char *p ){
    // length modifiers are:
    // h, hh, l, ll, L, q, j, z, t
    return strspn( p, "hlLqjzt" );
}

ptrdiff_t 
parse_conversion_specifier( const char *p ){
    // conversion specifiers are:
    // d, i, o, u, x, X, e, E, f, F, g, G, a, A, c, C, s, S, p, n, m, %
    size_t d = strspn( p, "diouxXeEfFgGaAcCsSpnm%" );
    // This one is mandatory and there can only be one.
    assert( d==1 );
    return d;
}

void
archive( const char *p, ptrdiff_t span, char **q ){
    static char *empty = "";
    if( 0 == span ){
        *q = empty;
    }else{
        *q = calloc( span+1, 1 );
        strncpy( *q, p, span );
    }
}

bool
is( char *p, const char *q ){
    // return true if the strings are identical.
    size_t len = strlen( q );
    return (bool) ! strncmp( p, q, len );
}
static void
calc_actual_width( struct atom *a ){
    // FIXME Not (yet) supporting 'n' as a conversion specifier.
    // Reproduces the big table at 
    // https://en.cppreference.com/w/c/io/fprintf
/*
    length      conversion
    modifier    specifier       type
    --------------------------------
    (none)      c               int
    l           c               wint_t
    (none)      s               char*
    l           s               wchar_t*
    hh          d/i             signed char [int]
    h           d/i             short [int]
    (none)      d/i             int
    l           d/i             long
    ll          d/i             long long
    j           d/i             intmax_t
    z           d/i             signed size_t [ssize_t]
    t           d/i             ptrdiff_t
    hh          o/x/X/u         unsigned char [int]
    h           o/x/X/u         unsigned short [int]
    (none)      o/x/X/u         unsigned int
    l           o/x/X/u         unsigned long
    ll          o/x/X/u         unsigned long long
    j           o/x/X/u         uintmax_t
    z           o/x/X/u         size_t
    t           o/x/X/u         unsigned ptrdiff_t [???]
    (none)/l    f/F/e/E/a/A/g/G double
    L           f/F/e/E/a/A/g/G long double
    (none)      p               void*
*/
    static char buf[4097]; 
    if( is(a->conversion_specifier, "c") ){
        if( is( a->length_modifier, "" ) ){
            snprintf( buf, 4096, a->original_specification, va_arg( *(a->pargs), int ) );
        }else if( is( a->length_modifier, "l" ) ){
            snprintf( buf, 4096, a->original_specification, va_arg( *(a->pargs), wint_t ) );
        }else{
            assert(0);
        }
    }else if( is(a->conversion_specifier, "s") ){
        if( is( a->length_modifier, "" ) ){
            snprintf( buf, 4096, a->original_specification, va_arg( *(a->pargs), char* ) );
        }else if( is( a->length_modifier, "l" ) ){
            snprintf( buf, 4096, a->original_specification, va_arg( *(a->pargs), wchar_t* ) );
        }else{
            assert(0);
        }
    }else if( is(a->conversion_specifier, "d") 
          ||  is(a->conversion_specifier, "i") ){
        if( is( a->length_modifier, "hh" ) ){
            snprintf( buf, 4096, a->original_specification, va_arg( *(a->pargs), int ) );
        }else if( is( a->length_modifier, "h" ) ){
            snprintf( buf, 4096, a->original_specification, va_arg( *(a->pargs), int ) );
        }else if( is( a->length_modifier, "" ) ){
            snprintf( buf, 4096, a->original_specification, va_arg( *(a->pargs), int ) );
        }else if( is( a->length_modifier, "l" ) ){
            snprintf( buf, 4096, a->original_specification, va_arg( *(a->pargs), long ) );
        }else if( is( a->length_modifier, "ll" ) ){
            snprintf( buf, 4096, a->original_specification, va_arg( *(a->pargs), long long ) );
        }else if( is( a->length_modifier, "j" ) ){
            snprintf( buf, 4096, a->original_specification, va_arg( *(a->pargs), intmax_t ) );
        }else if( is( a->length_modifier, "z" ) ){
            snprintf( buf, 4096, a->original_specification, va_arg( *(a->pargs), ssize_t ) );
        }else if( is( a->length_modifier, "t" ) ){
            snprintf( buf, 4096, a->original_specification, va_arg( *(a->pargs), ptrdiff_t ) );
        }else{
            assert(0);
        }
    }else if( is(a->conversion_specifier, "o") 
          ||  is(a->conversion_specifier, "x") 
          ||  is(a->conversion_specifier, "X") 
          ||  is(a->conversion_specifier, "u") ){
        if( is( a->length_modifier, "hh" ) ){
            snprintf( buf, 4096, a->original_specification, va_arg( *(a->pargs), int ) );
        }else if( is( a->length_modifier, "h" ) ){
            snprintf( buf, 4096, a->original_specification, va_arg( *(a->pargs), int ) );
        }else if( is( a->length_modifier, "" ) ){
            snprintf( buf, 4096, a->original_specification, va_arg( *(a->pargs), unsigned int ) );
        }else if( is( a->length_modifier, "l" ) ){
            snprintf( buf, 4096, a->original_specification, va_arg( *(a->pargs), unsigned long ) );
        }else if( is( a->length_modifier, "ll" ) ){
            snprintf( buf, 4096, a->original_specification, va_arg( *(a->pargs), unsigned long long ) );
        }else if( is( a->length_modifier, "j" ) ){
            snprintf( buf, 4096, a->original_specification, va_arg( *(a->pargs), uintmax_t ) );
        }else if( is( a->length_modifier, "z" ) ){
            snprintf( buf, 4096, a->original_specification, va_arg( *(a->pargs), size_t ) );
        }else if( is( a->length_modifier, "t" ) ){
            snprintf( buf, 4096, a->original_specification, va_arg( *(a->pargs), ptrdiff_t ) );
        }else{
            assert(0);
        }
    }else if( is(a->conversion_specifier, "f") 
          ||  is(a->conversion_specifier, "F") 
          ||  is(a->conversion_specifier, "e") 
          ||  is(a->conversion_specifier, "E") 
          ||  is(a->conversion_specifier, "a") 
          ||  is(a->conversion_specifier, "A") 
          ||  is(a->conversion_specifier, "g") 
          ||  is(a->conversion_specifier, "G") ){
        if( is( a->length_modifier, "l" )
        ||  is( a->length_modifier, "" ) ){
            snprintf( buf, 4096, a->original_specification, va_arg( *(a->pargs), double) );
        }else if( is( a->length_modifier, "L" ) ){
            snprintf( buf, 4096, a->original_specification, va_arg( *(a->pargs), long double) );
        }else{
            assert(0);
        }
    }else if( is(a->conversion_specifier, "p") ){
        if( is( a->length_modifier, "" ) ){
            snprintf( buf, 4096, a->original_specification, va_arg( *(a->pargs), void* ) );
        }else{
            assert(0);
        }
    }else{
        assert(0);
    }
    assert( strnlen(buf, 4096) < 4095 );
    a->original_field_width = strlen( buf );
}

void
cprintf( const char *fmt, ... ){
    va_list args;
    struct atom *a;
    const char *p = fmt, *q = fmt;
    ptrdiff_t d = 0;
    ptrdiff_t span;

    va_start( args, fmt );
    while( *p != '\0' ){
        d = strcspn( p, "%" ); 
        q = p;
        if( d == 0 ){
            // We've found a converstion specification.
            a = create_atom( false );
            a->pargs = &args;
            a->is_conversion_specification = true;

            q++; // Skip over initial '%'

            span = parse_flags( q );
            archive( q, span, &(a->flags) );
            q += span;
            
            span = parse_field_width( q );
            archive( q, span, &(a->field_width) );
            q += span;

            span = parse_precision( q );
            archive( q, span, &(a->precision) );
            q += span;

            span = parse_length_modifier( q );
            archive( q, span, &(a->length_modifier) );
            q += span;

            span = parse_conversion_specifier( q );
            archive( q, span, &(a->conversion_specifier) );
            q += span;

            archive( p, q-p, &(a->original_specification) ); 

            calc_actual_width( a );

            p = q;
        }else{
            // We've found some normal text.
            a = create_atom( false );
            a->is_conversion_specification = false;
            archive( q, d, &(a->ordinary_text) );
            q += d;
            p = q;
        }
    }
    va_end(args);
}

