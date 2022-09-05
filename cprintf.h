#include <stdbool.h>

struct atom{
    bool populated;
    bool is_conversion_specification;

    // parts of the conversion specification.
    const char *str_flags;
    const char *str_field_width;
    const char *str_precision;
    const char *str_length_modifier;
    const char *str_specifier;
    
    // our modifications
    size_t new_field_width;
    char *str_new_field_width;

    // use this for parts of the format string that aren't conversion specifications.
    const char *text;

    // id (all zero-indexed)
    size_t row;
    size_t col;
    size_t count;

    // navigation
    struct atom *right;
    struct atom *left;
    struct atom *up;
    struct atom *down;
};

void dump_graph( void );
void dump_atom( struct atom * a );
struct atom * create_atom( bool is_newline );
bool ischartoken( const char * const restrict p, const char c, const char ** restrict q );
bool isstrtoken( const char * const restrict p, const char * const restrict s, const char ** restrict q );

