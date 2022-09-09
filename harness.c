#include <stdio.h>
#include <assert.h>
#include <stdbool.h>
#include "cprintf.h"

int main(){
    struct atom *a1, *a2, *a3, *a4, *a5, *a6, *a7, *a8, *a9;
    a1 = create_atom( false );
    a1->original_specification = "my first atom.";
    a2 = create_atom( false );
    a2->original_specification = "my second atom.";
    a3 = create_atom( false );
    a3->original_specification = "my third atom.";

    printf("a3->original_specification = %s\n", a3->original_specification );
    printf("a3->left->original_specification = %s\n", a3->left->original_specification );
    printf("a3->left->left->original_specification = %s\n", a3->left->left->original_specification );

    printf("---\n");

    printf("a1->original_specification = %s\n", a1->original_specification );
    printf("a1->right->original_specification = %s\n", a1->right->original_specification );
    printf("a1->right->right->original_specification = %s\n", a1->right->right->original_specification );

    printf("---\n");

    dump_graph();

    printf("---\n");

    a4 = create_atom( true );
    a4->original_specification = "2nd line my first atom.";
    a5 = create_atom( false );
    a5->original_specification = "2nd line my second atom.";
    a6 = create_atom( false );
    a6->original_specification = "2nd line my third atom.";

    a7 = create_atom( true );
    a7->original_specification = "3rd line my first atom.";
    a8 = create_atom( false );
    a8->original_specification = "3rd line my second atom.";
    a9 = create_atom( false );
    a9->original_specification = "3rd line my third atom.";


    dump_graph();

    return 0;
}

