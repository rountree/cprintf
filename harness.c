#include <stdio.h>
#include <assert.h>
#include <stdbool.h>
#include "cprintf.h"

void
ischartoken_test(){
    const char * const restrict p = "Hello, world!\n";
    const char * q = NULL;
    bool rc;

    q = NULL;
    rc = ischartoken( p, 'H', &q );
    assert( rc == true );
    assert( q == p+1 );

    q = NULL;
    rc = ischartoken( p, 'e', &q );
    assert( rc == false );
    assert( q == p );

    q = NULL;
    rc = ischartoken( p, 'H', NULL );
    assert( rc == true );
    assert( q == NULL );

    q = NULL;
    rc = ischartoken( p, 'e', NULL );
    assert( rc == false );
    assert( q == NULL );
}

void
isstrtoken_test(){
    const char * const restrict p = "Hello, world!\n";
    const char * q = NULL;
    bool rc;

    q = NULL;
    rc = isstrtoken( p+2, "ll", &q );
    assert( rc == true );
    assert( q == p+4 );

    q = NULL;
    rc = isstrtoken( p+2, "qq", &q );
    assert( rc == false );
    assert( q == p+2 );

    q = NULL;
    q = NULL;
    rc = isstrtoken( p+2, "ll", NULL );
    assert( rc == true );
    assert( q == NULL );

    q = NULL;
    rc = isstrtoken( p+2, "qq", NULL );
    assert( rc == false );
    assert( q == NULL );
}

int main(){
    ischartoken_test();
    isstrtoken_test();
    return 0;
}

