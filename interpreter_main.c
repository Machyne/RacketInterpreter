/*| Submitted by:
|*| Matt Cotter (Not here to object to not object to being one of the walrii)
|*| Greg Fournier (walrii)
|*| Joy Hill (walrii)
|*/
#define VECTOR_SIZE 20

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "common.h"
#include "parstack.h"
#include "vector.h"
#include "interpreter.h"
#include "primitives.h"


/* The extern qualifier here means that tokens is defined in
   another file. */
extern ConsCell *tokens;
extern ConsCell *stack;

Vector *closureGarbage, *envGarbage, *consGarbage, *valGarbage;
void cleanAllGarbage();

int main() {
    bool REPL = isatty(fileno(stdin));

    closureGarbage = (Vector *)malloc(sizeof(Vector));
    envGarbage = (Vector *)malloc(sizeof(Vector));
    consGarbage = (Vector *)malloc(sizeof(Vector));
    valGarbage = (Vector *)malloc(sizeof(Vector));
    initVector(envGarbage, VECTOR_SIZE * sizeof(void*));
    initVector(closureGarbage, VECTOR_SIZE * sizeof(void*));
    initVector(consGarbage, VECTOR_SIZE * sizeof(void*));
    initVector(valGarbage, VECTOR_SIZE * sizeof(void*));


    // NOTE !! negative one depth means error :)
    int depth = 0; //aka "Count of open parens yet to be closed" aka "(!) Count"
    stack = NULL;
    Frame* topEnv = makeFrame(NULL);
    insertVectorItem(envGarbage, 0, (void *)topEnv);
    addPrimitives(topEnv);
    
    if(REPL) printf("> ");
    while (yylex()) {
        if (tokens == NULL) {
            // input is a empty line, skip
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
                Value evaluated = eval(parseTree, topEnv);
                if(evaluated.type == openType){
                    errorFlag = true;
                    printf("evaluation error\n");
                }else if(evaluated.type != voidType){
                    printParseTree(evaluated);
                    printf("\n");
                }
            }
            freeConsCell(stack);
            stack = NULL;
            if(errorFlag){
                depth = -1;
                break;
            }
        }
        if(REPL){
            printf("%s", (depth==0? "> ":"| "));
            for(int i=0; i<depth; i++) printf("  ");
        }
    }
    if(REPL && (depth>=0)) printf("\n");

    if (depth > 0) {
        printf("syntax error\n"); 
        fprintf(stderr, "There %s %i more open paren%s than close parens.\n", (depth==1?"was":"were"), depth, (depth==1?"":"s"));
    }
    cleanAllGarbage();
}





void cleanClosureGarbage(){
    void *pointerToAClosure = NULL;
    int vectorSize = closureGarbage->size;
    for(int i = 0; i < vectorSize; i++){
        getVectorItem(closureGarbage, i, &pointerToAClosure);
        freeValue( ((Closure *) pointerToAClosure)->body );
        freeValue( ((Closure *) pointerToAClosure)->formals );
        free((Closure *) pointerToAClosure);
    }
    cleanupVector(closureGarbage);
}

void cleanCCGarbage(){
    void *pointerToACC = NULL;
    int vectorSize = consGarbage->size;
    for(int i = 0; i < vectorSize; i++){
        getVectorItem(consGarbage, i, &pointerToACC);
        free((ConsCell *) pointerToACC);
    }
    cleanupVector(consGarbage);
}

void cleanValGarbage(){
    void *pointerToAValue = NULL;
    int vectorSize = valGarbage->size;
    for(int i = 0; i < vectorSize; i++){
        getVectorItem(valGarbage, i, &pointerToAValue);
        freeValue(*((Value *) pointerToAValue));
        free((Value *) pointerToAValue);
    }
    cleanupVector(valGarbage);
}

void cleanFrameGarbage(){
    void *pointerToAFrame = NULL;
    int vectorSize = envGarbage->size;
    for(int i = 0; i < vectorSize; i++){
        getVectorItem(envGarbage, i, &pointerToAFrame);
        cleanupFrame((Frame *) pointerToAFrame);
    }
    cleanupVector(envGarbage);
}

void cleanAllGarbage() {
    cleanClosureGarbage();
    cleanCCGarbage();
    cleanValGarbage();
    cleanFrameGarbage();
}
