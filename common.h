/* Common data structures for the Racket interpreter project. */
#ifndef __COMMON
#define __COMMON

#include <stdbool.h>
#include <string.h>
#include "vector.h"

typedef enum {
    booleanType,
    integerType,
    realType,
    openType,
    closeType,
    stringType,
    symbolType,
    consType,
    closureType,
    primitiveType,
    voidType,
    quotedSymbolType,
    quotedConsType    
} VALUE_TYPE;

enum __ParenType{
    parenPType, // paren(parenPType)Type i.e. ( ) // Recusived definititions are a good idea right?
    squarePType, // square Paren Type i.e. [ ]
    curlyPType // curly Paren Type i.e. { }
};

typedef struct __Value {
    VALUE_TYPE type;
    union {
        bool booleanValue;
        int integerValue;
        double realValue;
        char *stringValue;
        enum __ParenType parenValue;
        struct __ConsCell *consValue;
        char *symbolValue;
        struct __Value (*primitiveValue)(Vector args);
        struct __Closure *closureValue;
    };
} Value;

typedef struct __ConsCell {
    Value car;
    Value cdr;
} ConsCell;

/* lexing */
int yylex();


/*
 * Prints all of the values of the tokens in the linked
 * list starting with curr as the head.
 */
void printTokenList(ConsCell *curr);


/*
 * This frees the memory of the entire linked list
 * starting with cc as the head. This also works if
 * it is an improper list // ex (3 . 4)
 */
void freeConsCell(ConsCell *cc);

/*
 * If Value has pointers, this frees those :)
 */
void freeValue(Value val);

#endif
