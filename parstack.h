/* Header file for the parsing stack code. */

#ifndef __PARSTACK
#define __PARSTACK

#include <stdbool.h>
#include <string.h>
#include "common.h"

/*
 *  Push all tokens onto stack
 *      when a ')' is encountered
 *          pop off tokens until an open paren is popped
 *          then create a list of all things between the two parens
 *      push the list back onto the stack
 *  In theory, when the parse is complete, this newly added list
 *  will be the only item on the stack. 
 */
void parse(int* depth);

/*
 * Pushes an item onto the stack
 */
void push(Value val, ConsCell **theStack);

/*
 * Pops an item off the stack
 */
Value pop(ConsCell **theStack);

/*
 * Returns the parse tree, aka the first (only) item (sExpression)
 * currently on the stack (because depth is 0 and we hit a newline)
 */
Value getParseTree(bool *errorFlag);

/*
 * This prints S expressions recursively. The ouput will
 * look very similar to the Racket code.
 *
 * A very dandy helper function!
 */
void printSExpression(ConsCell *tree);

/*
 * This prints quoted S expressions recursively. The ouput
 * will look very similar to the Racket output minus nested
 * quotes because those are weird.
 *
 * A very dandy helper function!
 */
void printQSExpression(ConsCell *tree);

/*
 * This prints the parse tree. The ouput will look
 * very similar to the Racket code.
 */
void printParseTree(Value parseTree);

#endif
