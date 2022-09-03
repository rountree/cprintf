/* cprintf.c
 * Author:  Barry Rountree, rountree@llnl.gov
 */

/* BNF Grammar for printf format strings.
 * Taken from the printf(3) man page, Linux man-pages project, v3.53
 *
 * + means 1 or more
 * ? means 0 or 1
 * * means 0 or more
 * | means "or"
 *
 * UPPER_CASE indicatees a terminal token.
 * Parentheses are used for grouping and are not themselves tokens.
 * Square brackets indicate a range of characters
 *
 * format_str -> ((ordinary_characters)*|(conversion_specification)*)*
 * 
 * ordinary_characters -> (anything except a PERCENT)*
 *
 * conversion_specification ->
 *  PERCENT flags* field_width? precision? length_modifier? conversion_specifier
 *
 * flags -> OCTOTHORPE | ZERO | MINUS | SPACE | PLUS | APOSTROPHE | UC_I
 *
 * field_width -> number | ASTERISK | ASTERISK number DOLLAR
 *
 * precision -> PERIOD ( number | ASTERISK | ASTERISK number DOLLAR )?
 *
 * length_modifier -> LC_double_h  | LC_h | LC_l | LC_double_l | UC_L | LC_q | 
 *  LC_j | LC_z | LC_t 
 *
 * conversion_specifier -> LC_d | LC_i | LC_o | LC_u | LC_x | UC_X | LC_e | 
 *  UC_E | LC_f | UC_F | LC_g | UC_G | LC_a | UC_A | LC_c | LC_s | UC_C | 
 *  UC_S | LC_p | LC_n | LC_m | PERCENT
 *
 * number -> [1-9][0-9]*
 */

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <inttypes.h>
#include <assert.h>
#include <stdarg.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

union values{
    unsigned char hh;
    unsigned short int h;
    unsigned long int l;
    unsigned long long int ll;
    unsigned long double L;
    unsigned long long int q;
    uintmax_t j;
    size_t z;
    ptrdiff_t t;
};

struct atom{

    // If is_conversion_specification is true, str_original holds a copy
    // of a complete conversion specifiation, beginning with % and ending
    // with a conversion specifier.  If false, it holds a copy of the string
    // that appears before, after, or between conversion specifications, and
    // the rest of the struct is unused.
    bool is_conversion_specification;

    // original_atom holds a pointer to a copy of the original section
    // of the format string.
    char *str_original;

    // bit array to keep track of what we've parsed.
    uint64_t bits;
    size_t width;
    size_t precision;

    char *str_flags;
    char *str_width;
    char *str_new_width;
    char *str_precision;
    char *str_length_modifier;
    char *str_conversion_specifier;

    union values val;

    struct atom *next_atom_in_row;
    struct atom *next_atom_in_col;
};

// Pointer to the first atom (first column of the first row).
static struct atom *head = NULL;
static bool new_row = FALSE;
typedef enum token{
    PERCENT         = 0x00,
    // flags
    START_FLAGS,
    FLAG_OCTOTHORPE = START_FLAGS,
    FLAG_ZERO,
    FLAG_MINUS,
    FLAG_SPACE,
    FLAG_PLUS,
    FLAG_APOSTROPHE,
    FLAG_UC_I,
    END_FLAGS = FLAG_UC_I,
    // field width
    START_WIDTH,
    WIDTH_ASTERISK = START_WIDTH,
    WIDTH_DOLLAR,
    WIDTH_NUMBER,
    END_WIDTH = WIDTH_NUMBER,
    // precision
    START_PRECISION,
    PRECISION_PERIOD = START_PRECISION,
    END_PRECISION = PRECISION_PERIOD,
    // length modifier
    START_LENMOD,
    LENMOD_hh = START_LENMOD,
    LENMOD_h,
    LENMOD_l,
    LENMOD_ll,
    LENMOD_L,
    LENMOD_q,
    LENMOD_j,
    LENMOD_z,
    LENMOD_t,
    END_LENMOD = LENMOD_t,
    // conversion specifier
    START_CONVSPEC,
    CONVSPEC_d = START_CONVSPEC,
    CONVSPEC_i,
    CONVSPEC_o,
    CONVSPEC_u,
    CONVSPEC_x,
    CONVSPEC_X,
    CONVSPEC_e,
    CONVSPEC_E,
    CONVSPEC_f,
    CONVSPEC_F,
    CONVSPEC_g,
    CONVSPEC_G,
    CONVSPEC_a,
    CONVSPEC_A,
    CONVSPEC_c,
    CONVSPEC_s,
    CONVSPEC_C,
    CONVSPEC_S,
    CONVSPEC_p,
    CONVSPEC_n,
    CONVSPEC_m,
    END_CONVSPEC = CONVSPEC_m,
    NUM_TOKENS
};

struct atom * 
add_atom( ){
    // This is the new atom
    struct atom *a = calloc( sizeof( struct atom ), 1);
    // This is a temporary atom pointer
    struct atom *t = head;
    struct atom *above_t = NULL;    // silly but effective

    assert(a != NULL);

    // Can't actually rely on NULL having the value of 0.
    a->str_original             = NULL; 
    a->str_width                = NULL;
    a->str_new_width            = NULL;
    a->str_precision            = NULL;
    a->str_length_modifier      = NULL;
    a->str_conversion_specifier = NULL;

    if (head == NULL){
        // Handle the case where this is the first atom
        // new_row is true in this case
        head = a;
        head->next_atom_in_row = NULL;
        head->next_atom_in_col = NULL;
    }else{
        // find the last row
        while( t->next_atom_in_col != NULL ){
            t = t->next_atom_in_col;
        }
        // add a new row if necessary
        if (new_row){
            t->next_atom_in_col = a;
            t = a;
        }
        // find the last column
        while( t->next_atom_in_row != NULL ){
            t = t->next_atom_in_row;
        }
        // add our new atom
        t->next_atom_in_row = a;

        // bookkeeping
        a->next_atom_in_col = NULL;
        a->next_atom_in_row = NULL;
    } 


    // Now we want the atom in the row above this one
    // to point down to our new atom.
    t = head;
    above_t = head;
    if( t->next_atom_in_row == NULL ){
        // There's only one row.  We're done.
    }else{
        t = t->next_atom_in_col;
        while( t->next_atom_in_col != NULL ){
            above_t = t;
            t = t->next_atom_in_col;
        }
        // Once we're here, t points to the atom in the first column of the 
        // last row and t_above points to the atom in the first column of
        // the next-to-last row.  Now walk down both rows.  We're relying on
        // each call to cprintf in between calls to cflush having the same
        // number of atoms.
        while( t->next_atom_in_row ){
            t = t->next_atom_in_row;
            t_above = t_above->next_atom_in_row;
            assert( t_above != NULL );
        }
        t_above->next_atom_in_col = t;
    }

    // reset to true on each new call to cprintf
    new_row = false;    
    return a;
}

void
archive( char **dest, char *p, char *q ){
    // allocate memory in dest for a NULL-terminated string
    // and then copy the string from p to q inclusive to dest.
    *dest = calloc( 1, (q-p)+2 );
    memcpy( *dest, p, (q-p)+1 );
}

bool 
ischartoken( const char const *p, char c, char **q ){
    bool rc = false;
    if( *p == c ){
        rc = true;
    }
    if( **q != NULL ){
        *q = p+rc;
    }
    return rc;
}

bool
isstrtoken( const char const *p, const char const *s, char **q ){
    bool rc = false;
    if( **q != NULL
       
}

char *
parse_flags( char *p, struct atom *a ){
}

char *
parse_field_width( char *p, struct atom *a ){
}

char *
parse_precision( char *p, struct atom *a ){

}

char *
parse_length_modifier( char *p, struct atom *a ){

}

char *
parse_conversion_specifier( char *p, struct atom *a ){

}

char *
parse_conversion_specification( char *p ){
}

char *
parse_regular_text( char *p ){
}

int
cprintf( const char const *fmt, ... ){

    char *p=fmt, *q=fmt;
    new_row = true; 
    if( fmt == NULL ){
        // Don't parse NULL pointers.
        return 0;
    }

    if( strlen(fmt) == 0 ){
        // Don't parse empty strings.
        return 0;
    }

    // The main loop.
    while( *q != 0 ){
        if( istoken( q, PERCENT ) ){
            q = parse_conversion_specification( p+1, q );
        }else{
            q = parse_regular_text( p, q );
        }
        q++;
    }
    return 0;
}

