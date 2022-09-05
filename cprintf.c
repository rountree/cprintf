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

struct atom *
create_atom( bool is_newline ){
    /* "Creating" an atom means grabbing the next unpopulated atom and creating
     * new atoms below and to the right of it, then updating the links.*/
    static struct atom *next_unpopulated_atom = NULL;
    static struct atom *next_unpopulated_line = NULL;

    struct atom *a;

    if( NULL == origin ){
        origin = calloc( sizeof( struct atom ), 1 );
        assert(origin);
        a = next_unpopulated_atom = next_unpopulated_line = origin;
    }else if( false == is_newline ){
        assert( next_unpopulated_atom );
        a = next_unpopulated_atom;
    }else{
        assert( next_unpopulated_line );
        a = next_unpopulated_line;
    }

    // At this point "a" should be a previously-allocated, unpopulated atom.
    assert( false == a->populated );

    // Populating
    a->populated = true;

    // RIGHT
    if( NULL == a->right ){
        a->right = calloc( sizeof( struct atom ), 1 );
        assert( a->right );
    }
    if( NULL == a->right->left ){
        a->right->left = a;
    }
    assert( a->right->left == a );

    // DOWN
    if( NULL == a->down ){
        a->down = calloc( sizeof( struct atom ), 1 );
        assert( a->down );
    }
    if( NULL == a->down->up ){
        a->down->up = a;
    }
    assert( a->down->up = a );

    // corner case
    if( NULL != a->left ){
        assert( a->left->down );
        assert( a->down );
        a->left->down->right = a->down;
        a->down->left = a->left->down;
    }

    // update statics
    if( is_newline ){
        next_unpopulated_line = a->down;
    }
    next_unpopulated_atom = a->right;

    // That's it!
    return a;    
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


