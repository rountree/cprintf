#include <stdbool.h>    // true and false
#include <stddef.h>     // NULL
#include <assert.h>     // assert
#include <string.h>     // strncmp
#include <stdlib.h>     // calloc
#include <stdio.h>      // fprintf
#include <string.h>     // strspan
#include "cprintf.h"

struct atom{
    bool populated;
    bool is_conversion_specification;
    size_t original_field_width;
    size_t new_field_width;

    char *original_specification;
    char *new_specification;
    char *flags;
    char *field_width;
    char *precision;
    char *length_modifier;
    char *conversion_specifier;
    char *ordinary_text;
    
    // navigation
    struct atom *right;
    struct atom *left;
    struct atom *up;
    struct atom *down;
};

static struct atom * origin = NULL;



void
dump_atom( struct atom * a ){
    fprintf(stdout, "%s:%d tag=%zu a=%10p a->up=%10p a->down=%10p a->left=%10p a->right=%10p\n",
            __FILE__, __LINE__, a->new_field_width, a, a->up, a->down, a->left, a->right );
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

void
cprintf( const char *fmt, ... ){
    struct atom *a;
    const char *p = fmt, *q = fmt;
    ptrdiff_t d = 0;
    ptrdiff_t span;
    while( *p != '\0' ){
        d = strcspn( p, "%" ); 
        q = p;
        if( d == 0 ){
            // We've found a converstion specification.
            a = create_atom( false );
            a->is_conversion_specification = true;

            q++; // Skip over initial '%'
            fprintf( stdout, "%s:%d *q = '%c'\n", __FILE__, __LINE__, *q );

            span = parse_flags( q );
            archive( q, span, &(a->flags) );
            q += span;
            fprintf( stdout, "%s:%d a->flags=\"%s\"\n", __FILE__, __LINE__, a->flags );
            
            span = parse_field_width( q );
            archive( q, span, &(a->field_width) );
            q += span;
            fprintf( stdout, "%s:%d a->field_width=\"%s\"\n", __FILE__, __LINE__, a->field_width );

            span = parse_precision( q );
            archive( q, span, &(a->precision) );
            q += span;
            fprintf( stdout, "%s:%d a->precision=\"%s\"\n", __FILE__, __LINE__, a->precision );

            span = parse_length_modifier( q );
            archive( q, span, &(a->length_modifier) );
            q += span;
            fprintf( stdout, "%s:%d a->length_modifier=\"%s\"\n", __FILE__, __LINE__, a->length_modifier );

            span = parse_conversion_specifier( q );
            archive( q, span, &(a->conversion_specifier) );
            q += span;
            fprintf( stdout, "%s:%d a->conversion_specifier=\"%s\"\n", __FILE__, __LINE__, a->conversion_specifier );

            archive( p, (q-p)+1, &(a->original_specification) ); 
            fprintf( stdout, "%s:%d a->original_specification=\"%s\"\n", __FILE__, __LINE__, a->original_specification );
            fprintf( stdout, "\n");

            p = q;
        }else{
            // We've found some normal text.
            a = create_atom( false );
            a->is_conversion_specification = false;
            archive( q, d, &(a->ordinary_text) );
            q += d;
            fprintf( stdout, "%s:%d a->ordinary_text=\"%s\"\n", __FILE__, __LINE__, a->ordinary_text );
            p = q;
        }
    }
}



