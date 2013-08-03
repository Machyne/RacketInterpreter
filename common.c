/*| Submitted by:
|*| Matt Cotter (might be a walrus)
|*| Greg Fournier
|*| Joy Hill
|*/

#include "common.h"
#include <stdio.h>
#include <stdlib.h>

/*
 * Prints all of the values of the tokens in the linked
 * list starting with curr as the head.
 */
void printTokenList(ConsCell *curr){    
    if(!curr)return;
    switch(curr->car.type){
       case(booleanType):
         printf(curr->car.booleanValue?"Bool: true\n":"Bool: false\n");
         break;
        case(integerType):
           printf("Integer: %i\n",curr->car.integerValue);
           break;
        case(realType):
           printf("Real: %f\n",curr->car.realValue);
           break;
        case(openType):
           printf(curr->car.parenValue == parenPType?"Open: (\n":(curr->car.parenValue == squarePType?"Open: [\n":"Open: {\n"));
           break;
        case(closeType):
           printf(curr->car.parenValue == parenPType?"Close: )\n":(curr->car.parenValue == squarePType?"Close: ]\n":"Close: }\n"));
           break;
        case(stringType):
           printf("String: \"%s\"\n",curr->car.stringValue);
           break;
        case(symbolType):
           printf("Sybmol: %s\n",curr->car.symbolValue);
           break;
        default:
           fprintf(stderr, "Error: car is of type consType\n");
           break;   
    }
    if(curr->cdr.type != consType){
        //if the cdr is not a consCell
        fprintf(stderr, "Error: cdr is not of type consType\n");
        return;
    }
    printTokenList(curr->cdr.consValue);
}

/*
 * This frees the memory of the entire linked list
 * starting with cc as the head. This also works if
 * it is an improper list // ex (3 . 4)
 */
void freeConsCell(ConsCell *cc){
    if(!cc)return;
    freeValue(cc->car);
    freeValue(cc->cdr);
    free(cc);
}

/*
 * If Value has pointers, this frees those :)
 */
void freeValue(Value val){
    switch(val.type){
        case(stringType):
            free(val.stringValue);
            break;
        case(symbolType):
        case(quotedSymbolType):
            free(val.symbolValue);
            break;
        case(consType):
        case(quotedConsType):
            freeConsCell(val.consValue);
            break;
        default:
            break;
    }
}
