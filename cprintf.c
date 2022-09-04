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
    fprintf(stdout, "%s:%d tag=%zu a=%p a->up=%p a->down=%p a->left=%p a->right=%p\n",
            __FILE__, __LINE__, a->new_field_width, a, a->up, a->down, a->left, a->right );
}

struct atom *
create_atom( bool is_newline ){
    static struct atom *start_of_current_line = NULL;
    static struct atom *end_of_current_line = NULL;

    fprintf(stdout, "%s:%d on entry:  end_of_current_line=%p\n", 
            __FILE__, __LINE__, end_of_current_line);
   
    struct atom *a, *b, *d, *r; 

    if( NULL == origin ){
        /* a
         * d
         */
        a = calloc( sizeof(struct atom), 1 );
        assert(a);
        d = calloc( sizeof(struct atom), 1 );
        assert(d);

        origin = a;
        start_of_current_line = a;
        end_of_current_line = a;

        a->populated=true;
        a->right = NULL;
        a->left  = NULL;
        a->up    = NULL;
        a->down  = d;

        d->populated=false;
        d->right = NULL;
        d->left  = NULL;
        d->up    = a;
        d->down  = NULL;
        fprintf(stdout, "%s:%d end_of_current_line->down=%p\n",
                __FILE__, __LINE__, end_of_current_line->down);
    }else if( is_newline ){
        /* a
         * d
         */
        assert( start_of_current_line );
        assert( start_of_current_line->down );
        a = start_of_current_line->down;

        d = calloc( sizeof(struct atom), 1 );
        assert(d);

        start_of_current_line = a;
        end_of_current_line   = a;

        a->populated=true;
        a->right = NULL;
        a->left  = NULL;
        a->up    = NULL;
        a->down  = d;

        d->populated=false;
        d->right = NULL;
        d->left  = NULL;
        d->up    = a;
        d->down  = NULL;

    }else{
        /* a r
         * b d
         */
        a = end_of_current_line;
        assert(a);
        assert(a->populated);
        assert(a->down);

        b = a->down;
        assert(b);
        assert(!b->populated);
        assert(b->up);

        r = calloc( sizeof(struct atom), 1 );
        assert(r);
        d = calloc( sizeof(struct atom), 1 );
        assert(d);

        end_of_current_line = r;

        r->populated = true;

        a->right        = r;
        r->left         = a;
        r->down         = d;
        d->up           = r;
        d->left         = a->down;
        a->down->right  = d;
    }
    fprintf(stdout, "%s:%d on exit:   end_of_current_line=%p\n", 
            __FILE__, __LINE__, end_of_current_line);
    dump_atom(end_of_current_line);
    return end_of_current_line;
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


