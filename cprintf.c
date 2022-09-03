#include <stdbool.h>    // true and false
#include <stddef.h>     // NULL
#include <assert.h>     // assert
#include <string.h>     // strncmp
#include "cprintf.h"

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

