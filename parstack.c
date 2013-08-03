/*| Submitted by:
|*| Matt Cotter
|*| Greg Fournier (might be a walrus)
|*| Joy Hill
|*/

#include "common.h"
#include "parstack.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>


extern ConsCell *tokens;
ConsCell *stack;

/*
 *  Push all tokens onto stack
 *      when a ')' is encountered
 *          pop off tokens until an open paren is popped
 *          then create a list of all things between the two parens
 *      push the list back onto the stack
 *  In theory, when the parse is complete, this newly added list
 *  will be the only item on the stack. 
 */
void parse(int* depth){
    //ConsCell *curr = tokens;
    while(tokens){
        Value toPush = pop(&tokens);
        if(toPush.type != closeType){
            if(toPush.type == openType){
                *depth = (*depth) + 1;
            }
            push(toPush, &stack);
        }else{
            //We've found a close paren
            ConsCell *list = NULL;
            if(!stack){
                printf("syntax error\n");
                fprintf(stderr, "Parse Error: More close parens than open.\n");
                *depth = -1; //this means we are encoding an error.
                return;
            }
            Value popped = pop(&stack);
            while((popped.type != openType) && stack){
                //Add popped to the list
                push(popped, &list);
                popped = pop(&stack);
            }
            if(!stack && (popped.type != openType)){
                printf("syntax error\n");
                fprintf(stderr, "Parse Error: More close parens than open.\n");
                *depth = -1; //this means we are encoding an error.
                return;
            }
            if(toPush.parenValue != popped.parenValue){
                printf("syntax error\n");
                fprintf(stderr, "Parse Error: Mrs.Matched paren types.\n");
                *depth = -1; //this means we are encoding an error.
                return;                
            }
            Value sExpression;
            sExpression.type = consType;
            sExpression.consValue = list;
            push(sExpression, &stack);
            *depth = (*depth) - 1;
        }
    }
}

/*
 * Pushes an item onto the stack
 */
void push(Value val, ConsCell **theStack){
    ConsCell *newStack = (ConsCell *) malloc(sizeof(ConsCell));
    newStack->car = val;
    newStack->cdr.type = consType;
    newStack->cdr.consValue = *theStack;
    *theStack = newStack;
}

/*
 * Pops an item off the stack
 */
Value pop(ConsCell **theStack){
    Value ret = (*theStack)->car;
    ConsCell *temp = (*theStack);
    *theStack = (*theStack)->cdr.consValue;
    free(temp);
    return ret;
}


/*
 * Returns the parse tree, aka the first (only) item (sExpression)
 * currently on the stack (because depth is 0 and we hit a newline)
 */
Value getParseTree(bool *errorFlag){
    //make sure stack has only 1 item
    if(!stack){
        printf("backend error\n");
        fprintf(stderr, "Parse Error: The stack should not have been null.\n");
        *errorFlag = true;
    }
    Value car = stack->car;
    Value cdr = stack->cdr;
    if(cdr.consValue){
        printf("syntax error\n");
        fprintf(stderr, "Parse Error: Multiple top level <S> Expressions on the same line.\n");
        *errorFlag = true;
    }
    return car;
}

void printFloat(float f){
    if(f-floor(f) == 0.0f){
        printf("%g.0", f);
    }else{
        printf("%g", f);
    }
}

/*
 * This prints the parse tree. The ouput will look
 * very similar to the Racket code.
 */
void printParseTree(Value parseTree){
    switch(parseTree.type){
    case(booleanType):
        printf(parseTree.booleanValue? "#t" : "#f");
        break;
    case(integerType):
        printf("%i",parseTree.integerValue);
        break;
    case(realType):
        printFloat(parseTree.realValue);
        break;
    case(stringType):
        printf("\"%s\"",parseTree.stringValue);
        break;
    case(quotedSymbolType):
        printf("\'%s",parseTree.symbolValue);
        break;
    case(symbolType):
        printf("%s",parseTree.symbolValue);
        break;
    case(closureType):
    case(primitiveType):
        printf("#<procedure>");
        break;
    case(quotedConsType):
        if(!parseTree.consValue){
            printf("'()");
        }else{
            printQSExpression(parseTree.consValue);
        }
        break;
    case(consType):
        if(!parseTree.consValue){
            printf("()");
        }else{
            printSExpression(parseTree.consValue);
        }
        break;
    default:
        fprintf(stderr, "Parse Error: bad parse tree\n");
        break;
    }
}

/*
 * This prints S expressions recursively. The ouput will
 * look very similar to the Racket code.
 *
 * A very dandy helper function!
 */
void printSExpression(ConsCell *tree){
    if(!tree) return;
    printf("(");
    bool space = true;
    while(tree){
        if(space){
            space = false;
        }else{
            printf(" ");
        }
        Value car = tree->car;
        printParseTree(car);
        Value cdr = tree->cdr;
        if(cdr.type != consType){
            printf(". ");
            printParseTree(cdr);
            tree = NULL;
        }else{
            tree = cdr.consValue;
        }
    }
    printf(")");
}

/*
 * This prints quoted S expressions recursively. The ouput
 * will look very similar to the Racket output minus nested
 * quotes because those are weird.
 *
 * A very dandy helper function!
 */
void printQSExpression(ConsCell *tree){
    if(!tree) return;
    printf("'(");
    bool space = true;
    while(tree){
        if(space){
            space = false;
        }else{
            printf(" ");
        }
        Value car = tree->car;
        printParseTree(car);
        Value cdr = tree->cdr;
        if(cdr.type != consType){
            printf(" . ");
            printParseTree(cdr);
            tree = NULL;
        }else{
            tree = cdr.consValue;
        }
    }
    printf(")");
}
