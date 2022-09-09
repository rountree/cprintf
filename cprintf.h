#include <stddef.h>
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

    va_list *pargs;

    // navigation
    struct atom *right;
    struct atom *left;
    struct atom *up;
    struct atom *down;
};

void dump_atom( struct atom * a );
void dump_graph( void );
void free_graph( struct atom *a );
struct atom * create_atom( bool is_newline );
ptrdiff_t parse_flags( const char *p );
ptrdiff_t parse_field_width( const char *p );
ptrdiff_t parse_precision( const char *p );
ptrdiff_t parse_length_modifier( const char *p );
ptrdiff_t parse_conversion_specifier( const char *p );
void archive( const char *p, ptrdiff_t span, char **q );
bool is( char *p, const char *q );
void cprintf( const char* fmt, ... );


