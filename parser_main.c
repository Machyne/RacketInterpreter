/*| Submitted by:
|*| Matt Cotter 
|*| Greg Fournier (might be a walrus)
|*| Joy Hill
|*/

#include <stdio.h>
#include <stdlib.h>

#include "common.h"
#include "parstack.h"

/* The extern qualifier here means that tokens is defined in
   another file. */
extern ConsCell *tokens;
extern ConsCell *stack;

int main() {
    // NOTE !! negative one depth means error :)
    int depth = 0; //aka "Count of open parens yet to be closed" aka "(!) Count"
    stack = NULL;
    while (yylex()) {
        if (tokens == NULL) {
            // empty line, skip
            continue;
        }
        parse(&depth);
        if(depth == -1){
            break;
        }
        if (depth == 0) {
            bool errorFlag = false;
            Value parseTree = getParseTree(&errorFlag);
            if(!errorFlag){
                printParseTree(parseTree);
                printf("\n");
            }
            freeConsCell(stack);
            stack = NULL;
            if(errorFlag){
                depth = -1;
                break;
            }
        }
    }

    if (depth > 0) {
        printf("syntax error\n"); 
        fprintf(stderr, "There %s %i more open paren%s than close parens.\n", (depth==1?"was":"were"), depth, (depth==1?"":"s"));
    }
}

