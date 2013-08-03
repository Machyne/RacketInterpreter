/*| Submitted by:
|*| Matt Cotter (might be a walrus)
|*| Greg Fournier
|*| Joy Hill
|*/

#include <stdio.h>
#include <stdlib.h>

#include "common.h"

/* The extern qualifier here means that tokens is defined in
   another file. */
extern ConsCell *tokens;


extern void freeValue(Value val);
extern void freeConsCell(ConsCell *cc);
extern void printTokenList(ConsCell *curr);

int main(int argc, char **argv) {
    while (yylex()) {
        /* print the tokens for this line */
        printf("Lex tokenized a line.\n");
    }
    printTokenList(tokens);
    freeConsCell(tokens);
}
