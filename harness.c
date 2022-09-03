#include <stdio.h>
#include <assert.h>
#include <stdbool.h>
#include "cprintf.h"

void
ischartoken_test(){
    const char *p = "Hello, world!\n";
    const char *q = NULL;
    bool rc;

    rc = ischartoken( p, 'H', &q );
    assert( rc == true );
    assert( q == p+1 );

    rc = ischartoken( p, 'e', &q );
    assert( rc == false );
    assert( q == p );

    q = NULL;
    rc = ischartoken( p, 'H', NULL );
    assert( rc == true );
    assert( q == NULL );

    rc = ischartoken( p, 'e', NULL );
    assert( rc == false );
    assert( q == NULL );

}

int main(){
    ischartoken_test();
    return 0;
}

