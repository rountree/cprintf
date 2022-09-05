#include <stdbool.h>    // true and false
#include <stddef.h>     // NULL
#include <assert.h>     // assert
#include <string.h>     // strncmp
#include <stdlib.h>     // calloc
#include <stdio.h>      // fprintf
#include "cprintf.h"

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
    fprintf( stdout, "%s:%d node %p freed\n", __FILE__, __LINE__, a );
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

bool
ischartoken( const char * const restrict p, const char c, const char ** restrict q ){
    assert(p != NULL);
    bool rc = (*p == c);
    if( q != NULL ){
        // aka if(rc == true){ *q = p+sizeof(char); }else{ *q = p; }
        *q = p + rc;    
    }
    return rc;
}

bool
isstrtoken( const char * const restrict p, const char * const restrict s, const char ** restrict q ){
    assert( p != NULL );
    assert( s != NULL );
    bool rc = ! strncmp( p, s, strlen(s) );
    if( q != NULL ){
        // aka if(rc == true){ *q = p + strlen(s); }else{ *q = p; }
        *q = p + (rc * strlen(s));
    }
    return rc;
}


