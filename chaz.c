#include <stdio.h>
#include <string.h>


int main(){
    char *s = "Hello!";

    printf("s is a pointer to memory at address %p.\n", s);
    printf("If I dereference s, I get the value at that address.  *s = %c.\n", *s);
    printf("If I want the 2rd character, I can do pointer arithmetic:  *(s+2) = %c.\n", *(s+2) );
    printf("*(s+2) is equivalent to s[2].  s[2]=%c.\n", s[2]);
    printf("If i'm looking at *s+2, it's not obvious if that means (*s)+2 or *(s+2).\n");
    printf("The rule of thumb is that multiplication and division happen before addition and subtraction, and used paratheneses for everything else.\n");
    return 0;
}
