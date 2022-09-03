#include <stdbool.h>    // true and false
#include <stddef.h>     // NULL
#include <assert.h>
#include "cprintf.h"
bool
ischartoken( const char * const restrict p, const char c, const char ** restrict q ){
    assert(p != NULL);
    bool rc = (*p == c);
    if( q != NULL ){
        *q = p + rc;
    }
    return rc;
}

