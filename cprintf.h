#include <stddef.h>
#include <wchar.h>      // wint_t
#include <stdint.h>     // intmax_t

// These are the types that printf and friends are aware of.
// Recall that types shorter than int (e.g., char and short)
// are promoted to int when accessed by va_arg().
typedef union{
    int                 c_int;
    wint_t              c_wint_t;
    char*               c_charx;
    wchar_t*            c_wchar_tx;
    long                c_long;
    long long           c_long_long;
    intmax_t            c_intmax_t;
    ssize_t             c_ssize_t;
    ptrdiff_t           c_ptrdiff_t;
    unsigned int        c_unsigned_int;
    unsigned long       c_unsigned_long;
    unsigned long long  c_unsigned_long_long;
    uintmax_t           c_uintmax_t;
    size_t              c_size_t;
    double              c_double;
    long double         c_long_double;
    void*               c_voidx;
}value;

typedef enum{
    C_INT,
    C_WINT_T,
    C_CHARX,
    C_WCHAR_TX,
    C_LONG,
    C_LONG_LONG,
    C_INTMAX_T,
    C_SSIZE_T,
    C_PTRDIFF_T,
    C_UNSIGNED_INT,
    C_UNSIGNED_LONG,
    C_UNSIGNED_LONG_LONG,
    C_UINTMAX_T,
    C_SIZE_T,
    C_DOUBLE,
    C_LONG_DOUBLE,
    C_VOIDX
}type_t;

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
    type_t type;
    value  val;

    // navigation
    struct atom *right;
    struct atom *left;
    struct atom *up;
    struct atom *down;
};

void dump_atom( struct atom * a );
void dump_graph( void );
void dump_graph_2( void );
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
void cflush( void );

