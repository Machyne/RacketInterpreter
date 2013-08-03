/*| Submitted by:
|*| Matt Cotter (Not here to object to not object to being one of the walrii)
|*| Greg Fournier (walrii)
|*| Joy Hill (walrii)
|*/

#include "common.h"
#include "interpreter.h"
#include "parstack.h"
#include "vector.h"
#include <stdio.h>
#include <stdlib.h>



extern ConsCell *tokens;
extern Vector *closureGarbage, *envGarbage, *valGarbage;

// Error handling is done by passing up open paren types.
// Since there will never be an open type in the parse tree,
// it can be used to encode errors with little additional work.
Value getErrorValue(){
    Value openParenVal;
    openParenVal.type = openType;
    openParenVal.parenValue = curlyPType;
    return openParenVal;
}

// Returns a value of type void.
Value voidVal(){
    Value voidVal;
    voidVal.type = voidType;
    voidVal.integerValue = 0;
    return voidVal;
}

// Returns a value of type false
Value falseVal(){
    Value falseVal;
    falseVal.type = booleanType;
    falseVal.booleanValue = false;
    return falseVal;
}

//Prints the error message and returns an error-denoting open type.
Value evaluationError(char *errorMessage){
    if(errorMessage) fprintf(stderr, "Interpreter error: %s.\n",errorMessage);
    return getErrorValue();
    //We would do this: exit(1);
    //if the tests did not require nice exits.
}

// Evaluates the expression. In the frame that it is given
Value eval(Value expr, Frame *env) {
    Value toRet;
    switch (expr.type){
       case (booleanType): 
       case (integerType):
       case (realType):
       case (stringType):
        case (quotedConsType):
        case (quotedSymbolType):
        case (closureType):
        case (primitiveType):
            toRet = expr;
            break;
        case (symbolType):
            toRet = resolveVar(expr, env);
            break;
       case (consType): 
         if( !expr.consValue ){
                return evaluationError("empty expression");
            }else if(expr.consValue->car.type == consType){
                toRet = evalApply(expr, env);
                if(toRet.type == openType){
                    return evaluationError("first argument in an expression did not evaluate to a proceedure");
                }
                break;
            }else if(expr.consValue->car.type == symbolType){
                char *keyword = expr.consValue->car.symbolValue;

                if (strcmp(keyword, "if") == 0) {
                    toRet = evalIf(expr.consValue->cdr, env);

                } else if (strcmp(keyword, "quote") == 0){
                    toRet = evalQuote(expr.consValue->cdr, env);

                } else if (strcmp(keyword, "let") == 0){
                    toRet = evalLet(expr.consValue->cdr, env);

                } else if (strcmp(keyword, "define") == 0){
                    toRet = evalDefine(expr.consValue->cdr, env);

                } else if (strcmp(keyword, "lambda") == 0){
                    toRet = evalLambda(expr.consValue->cdr, env);

                } else if (strcmp(keyword, "let*") == 0){
                    toRet = evalLetStar(expr.consValue->cdr, env);

                } else if (strcmp(keyword, "letrec") == 0){
                    toRet = evalLetRec(expr.consValue->cdr, env);

                } else if (strcmp(keyword, "begin") == 0){
                    toRet = evalBegin(expr.consValue->cdr, env);

                } else if (strcmp(keyword, "cond") == 0){
                    toRet = evalCond(expr.consValue->cdr, env);
                } 
                else { 
                    /* not a recognized special form - must be user defined function */
                    toRet = evalApply(expr, env);
                }
                break;
            }else{
                return evaluationError("first argument in an expression was neither a symbol or expression");
            }
        case (voidType):
            toRet = evaluationError("cannot evaluate void type");
        default:
            toRet = evaluationError(NULL);
    }
    return toRet;
}

// Evaluates an If expression
Value evalIf(Value tree, Frame *env){
    Value test = getNth(tree.consValue,0); 
    Value trueExpr = getNth(tree.consValue,1); 
    Value falseExpr = getNth(tree.consValue,2);

    if( (test.type == openType) || (trueExpr.type == openType) || (falseExpr.type == openType) ){
        return evaluationError("not enough arguments given to an \'if\'");
    }

    Value testEv = eval(test,env);
    if(testEv.type != booleanType){
        //Bad.
        return evaluationError("first expression in an \'if\' did not evaluate to a boolean");
    }
    if(testEv.booleanValue){
        return eval(trueExpr,env);
    }else{
        return eval(falseExpr,env);
    }
}

// Evaluates a Quote expression
Value evalQuote(Value tree, Frame *env){
    if(tree.type != consType){
        return evaluationError("improper use of \'quote\'");
    }
    if(!tree.consValue){
        return evaluationError("no arguments given to \'quote\'");
    }
    if(tree.consValue->cdr.type != consType){
        return evaluationError("improper use of \'quote\'");
    }
    if(tree.consValue->cdr.consValue){
        return evaluationError("too many arguments given to \'quote\'");
    }

    Value car = tree.consValue->car;
    return quote(car);
}

// changes a given value to it "quoted" counterpart
Value quote(Value toQuote){
    switch (toQuote.type){
        case(booleanType): 
        case(integerType):
        case(realType):
        case(stringType):
            break;
        case(symbolType):
            toQuote.type = quotedSymbolType;
            break;
        case(consType):
            toQuote.type = quotedConsType;
            break;
        default:
            // This is either a parse error (paren) or eval error (nested quote??)
            return evaluationError("error in element to be quoted");
        break;
    }
    return toQuote;
}

// changes "quoted" value to its unquoted counterpart
Value unquote(Value quoted){
    switch (quoted.type){
        case(booleanType): 
        case(integerType):
        case(realType):
        case(stringType):
            break;
        case(quotedSymbolType):
            quoted.type = symbolType;
            break;
        case(quotedConsType):
            quoted.type = consType;
            break;
        default:
            return evaluationError("tried to \'unquote\' non quoted type");
        break;
    }
    return quoted;
}

// Evaluates a let expression and makes a new frame that will point to the frame that is given to it
// as a parameter
Value evalLet(Value tree, Frame *env){
    if(tree.type != consType){
        return evaluationError("improper use of \'let\'");
    }
    if(!tree.consValue){
        return evaluationError("no arguments given to \'let\'");
    }
    
    // Get the bindings and the body
    Value bindings = getNth(tree.consValue,0); 
    Value body = getNth(tree.consValue,1);
    if( (body.type == openType) || (bindings.type == openType) ){
        return evaluationError("not enough arguments given to a \'let\'");
    }

    // Make a new frame from the bindings list
    Frame *newEnv = makeFrame(env);
    insertVectorItem(envGarbage, envGarbage->size, (void *)newEnv);
    
    if(!addBindings(newEnv, bindings)){
        return evaluationError("poorly formed \'let\'");
    }

    return eval(body, newEnv);
}

// Evaluates a letRec expression and makes a new frame that will point to the frame that is given to it
// as a parameter
Value evalLetRec (Value tree, Frame *env) {
    if(tree.type != consType){
        return evaluationError("improper use of \'letrec\'");
    }
    if(!tree.consValue){
        return evaluationError("no arguments given to \'letrec\'");
    }
    
    // Get the bindings and the body
    Value bindings = getNth(tree.consValue,0); 
    Value body = getNth(tree.consValue,1);
    if( (body.type == openType) || (bindings.type == openType) ){
        return evaluationError("not enough arguments given to a \'letrec\'");
    }

    // Make a new frame from the bindings list
    Frame *newEnv = makeFrame(env);
    insertVectorItem(envGarbage, envGarbage->size, (void *)newEnv);
    
    if(!addBindingsRec(newEnv, bindings)){
        return evaluationError("poorly formed \'letrec\'");
    }

    return eval(body, newEnv);
}

// Evaluates a let* expression and makes a new frame that will point to the frame that is given to it
// as a parameter
Value evalLetStar(Value tree, Frame *env){
    if(tree.type != consType){
        return evaluationError("improper use of \'let*\'");
    }
    if(!tree.consValue){
        return evaluationError("no arguments given to \'let*\'");
    }
    
    // Get the bindings and the body
    Value bindings = getNth(tree.consValue,0); 
    Value body = getNth(tree.consValue,1);
    if( (body.type == openType) || (bindings.type == openType) ){
        return evaluationError("not enough arguments given to a \'let*\'");
    }

    // Make a new frame from the bindings list
    Frame *newEnv = makeFrame(env);
    insertVectorItem(envGarbage, envGarbage->size, (void *)newEnv);
    
    if(!addBindingsStar(newEnv, bindings)){
        return evaluationError("poorly formed \'let*\'");
    }

    return eval(body, newEnv);
}

// Evaluates a cond expression. Evaluates the cadr of the first thing with a car that evaluates to true
Value evalCond(Value tree, Frame *env) {
    
    int i = 0;
    // Get the bindings and the body
    Value val = getNth(tree.consValue, i); 

    // Check for errors 
    if(val.type == openType){
        return evaluationError("no arguments given to a \'cond\'");
    }

    // loop through the tree and return the
    while(val.type != openType){
        /////////////
        if( val.type != consType){ return evaluationError("poorly formed argument given to a \'cond\'"); }
        ConsCell *condCell = val.consValue;
        if( !condCell ){ return evaluationError("non test-value pair given to a \'cond\'"); }
        Value caar = getNth(condCell,0);
        Value cadar = getNth(condCell,1);
        /////////////
        i++;
        Value conditional = eval(caar, env);
        if(conditional.type != booleanType){
            return evaluationError("Non boolean value given to cond statement");
        }
        if(conditional.booleanValue == true){
            return eval(cadar, env);
        }
        val = getNth(tree.consValue, i);
    }
    return voidVal(); // or deal with else case

    //\\//\\//\\//\\//\\//\\//\\

    // Value ret = eval(val, env);
    // while(val.type != openType){
    //     ret = eval(val, env);
    //     if(ret.type == openType) break;
    //     i++;
    //     val = getNth(tree.consValue, i);
    // }
    
    // return ret;
}

// Evaluates a define expression and makes a new frame that will point to the frame that is given to it
// as a parameter
Value evalDefine(Value tree, Frame *env){
    // Get the bindings and the body
    Value val = getNth(tree.consValue,0); 
    Value expr = getNth(tree.consValue,1);
    // Check for errors
    if( (val.type == openType) || (expr.type == openType) ){
        return evaluationError("not enough arguments given to a \'define\'");
    }
    if(val.type != symbolType){
        return evaluationError("first argument given to \'define\' was not a symbol");
    }
    // End error checking

    //eval expr and add this binding
    Value *storedVal = (Value *)malloc(sizeof(Value));

    (*storedVal) = valdup(eval(expr,env));
    if((*storedVal).type == openType){
        free(storedVal);
        return evaluationError("second argument given to \'define\' could not be evaluated");
    }
    insertVectorItem(&(env->names), 0, (void *)strdup(val.symbolValue));
    insertVectorItem(&(env->values), 0, (void *)storedVal);
    
    return voidVal();
}

// allows us to evaluate user defined functions
Value evalApply(Value expr, Frame *env) {
    // evaluate function
    Value f = eval(expr.consValue->car, env);
    if( (f.type != closureType) && (f.type != primitiveType) ){
        return evaluationError("first argument of a function applcation did not evaluate to a proceedure");
    }

    // evaluate arguments
    Vector args;
    initVector(&args,8);
    int i = 1;
    Value curr = getNth(expr.consValue, i); 
    while(curr.type != openType){
        Value *storedVal = (Value *)malloc(sizeof(Value));
        (*storedVal) = valdup(eval(curr,env));
        if((*storedVal).type == openType){ //This is an error
            void *pointerToAVal = NULL;
            for(int j = 0; j < i-2; j++){
                getVectorItem(&args, j, &pointerToAVal);
                freeValue(*((Value *)pointerToAVal));
                free((Value *)pointerToAVal);
            }
            free(storedVal);
            return evaluationError(NULL);
        }
        insertVectorItem(&(args), i-1, (void *)storedVal);
        i++;
        curr = getNth(expr.consValue, i);
    }
    
    // apply the function
    Value ret = apply(f, args);
    cleanupVector(&args);
    return ret;
}

// Allows us to apply userdefined functions
Value apply(Value function, Vector args) {
    if (function.type == primitiveType) {
        void *pointerToAVal = NULL;
        for(int i = 0; i < args.size; i++){
            getVectorItem(&args, i, &pointerToAVal);
            insertVectorItem(valGarbage, valGarbage->size, pointerToAVal);
        }
        return function.primitiveValue(args);
    }
    if (function.type == closureType) {
        Frame *newEnv = makeFrame(function.closureValue->env);
        Value formals = function.closureValue->formals;
        
        //check length
        Value shouldBeLast = getNth(formals.consValue, args.size-1); //should work
        if(args.size == 0) shouldBeLast.type = closeType;
        Value shouldNotExist = getNth(formals.consValue, args.size); //should fail
        if( (shouldBeLast.type == openType) || (shouldNotExist.type != openType) ){
            return evaluationError("expected different number of args than recieved");
        }

        // bind formals to actuals
        for(int i = 0; i < args.size; i++){
            Value sym = getNth(formals.consValue, i);
            void *storedVal = NULL;
            getVectorItem(&args, i, &storedVal); 
            insertVectorItem(&(newEnv->names), 0, (void *)strdup(sym.symbolValue));
            insertVectorItem(&(newEnv->values), 0, (void *)storedVal);
        }
        insertVectorItem(envGarbage, envGarbage->size, (void *)newEnv);
        
        return eval(function.closureValue->body, newEnv);
    }
    // Else, error
    return evaluationError("apply called on non-function");
}

// Duplicates a value (recursively) and heap alocates all the pointers
Value valdup(Value value){
    switch(value.type){
        case(booleanType):
        case(integerType):
        case(realType):
        case(closureType):
        case(primitiveType):
            //All fine :)
            break;
        case(stringType):
        {
            char *dupped = strdup(value.stringValue);
            value.stringValue = dupped;
            break;
        }
        case(symbolType):
        case(quotedSymbolType):
        {
            char *dupped = strdup(value.symbolValue);
            value.symbolValue = dupped;
            break;
        }
        case(quotedConsType):
        case(consType):
        {
            if(!value.consValue){
                break;
            }
            ConsCell *cc = (ConsCell *) malloc(sizeof(ConsCell));
            cc->car = valdup(value.consValue->car);
            cc->cdr = valdup(value.consValue->cdr);
            value.consValue = cc;
            break;
        }
        default:
            return evaluationError(NULL); //passing an error back up
            //break;
    }
    return value;
}

// Evaluates a lambda statement and returns a new closure.
Value evalLambda(Value tree, Frame *env){
    //Get the args passed to the lambda
    Value formals = getNth(tree.consValue,0); 
    Value body = getNth(tree.consValue,1);
    if( (formals.type == openType) || (body.type == openType) ){
        return evaluationError("not enough arguments given to a \'lambda\'");
    }
    if(formals.type != consType){
        return evaluationError("first argument given to a \'lambda\' was not a list");
    }
    Closure closey;
    closey.env = env;
    
    int i = 1;
    Value curr = getNth(formals.consValue, i); 
    while(curr.type != openType){
        if(curr.type != symbolType){
            return evaluationError("formal parameters given to a \'lambda\' were not symbols");
        }
        i++;
        curr = getNth(formals.consValue, i);
    }

    closey.body = valdup(body);
    closey.formals = valdup(formals);

    Closure *storedClosure = (Closure *)malloc(sizeof(Closure));
    (*storedClosure) = closey;

    insertVectorItem(closureGarbage, closureGarbage->size, (void *)storedClosure);

    Value ret;
    ret.type = closureType;
    ret.closureValue = storedClosure;
    return ret;
}

// Evaluates a define expression and makes a new frame that will point to the frame that is given to it
// as a parameter
Value evalBegin(Value tree, Frame *env){
    int i = 0;
    // Get the bindings and the body
    Value val = getNth(tree.consValue, i); 

    // Returns void if no arguments are given
    if(val.type == openType){
        return voidVal();
    }

    Value ret;
    while(val.type != openType){
        ret = eval(val, env);
        if(ret.type == openType) return evaluationError(NULL);
        i++;
        val = getNth(tree.consValue, i);
    }
    
    return ret;
}

// Makes a new frame that points to its parent
// uses a Frame* so that the first one can point to NULL and it is prettier
Frame* makeFrame(Frame *parent) {
    // Create a frame
    // point new frame's parent pointer to parent
    Frame *newEnv = (Frame *)malloc(sizeof(Frame));
    newEnv->parent = parent;
    initVector(&(newEnv->names), 2*sizeof(void**));
    initVector(&(newEnv->values), 2*sizeof(void**));
    return newEnv;
}

// Used in conjunction with makeFrame. Adds bindings to the Frame theEnv from the Value bindings
// bindings is a linked list of pairs
// each pair has a name and a value, with the name first followed immediately by the value
bool addBindings(Frame *theEnv, Value bindings) {
    // add things
    ConsCell *cc = bindings.consValue;
    int i = 0;
    while(cc){
        if( cc->car.type != consType){ return false; }
        ConsCell *binding = cc->car.consValue;
        Value caar = getNth(binding,0);
        Value cadar = getNth(binding,1);
        if( (caar.type == openType) || (cadar.type == openType) ){
            return false;
        }
        Value *storedVal = (Value *)malloc(sizeof(Value));
        (*storedVal) = valdup(eval(cadar,theEnv->parent));
        if( (*storedVal).type == openType){
            free(storedVal);
            return false;
        }
        insertVectorItem(&(theEnv->names), i, (void *)strdup(caar.symbolValue));
        insertVectorItem(&(theEnv->values), i, (void *)storedVal);
        cc = cc->cdr.consValue;
        i++;
    }
    return true;
}

// Adds bindings to a frame. Used in let*
bool addBindingsStar(Frame *theEnv, Value bindings) {
    // add things
    ConsCell *cc = bindings.consValue;
    int i = 0;
    while(cc){
        if( cc->car.type != consType){ return false; }
        ConsCell *binding = cc->car.consValue;
        Value caar = getNth(binding,0);
        Value cadar = getNth(binding,1);
        if( (caar.type == openType) || (cadar.type == openType) ){
            return false;
        }
        Value *storedVal = (Value *)malloc(sizeof(Value));
        (*storedVal) = valdup(eval(cadar,theEnv));
        if( (*storedVal).type == openType){
            free(storedVal);
            return false;
        }
        insertVectorItem(&(theEnv->names), i, (void *)strdup(caar.symbolValue));
        insertVectorItem(&(theEnv->values), i, (void *)storedVal);
        cc = cc->cdr.consValue;
        i++;
    }
    return true;
}

// Adds bindings to a frame. Used in letrec
bool addBindingsRec(Frame *theEnv, Value bindings) {
    // add things
    ConsCell *cc = bindings.consValue;
    int i = 0;
    while(cc){
        if( cc->car.type != consType){ return false; }
        ConsCell *binding = cc->car.consValue;
        Value caar = getNth(binding,0);
        Value cadar = falseVal();
        if( (caar.type == openType) || (cadar.type == openType) ){
            return false;
        }
        Value *storedVal = (Value *)malloc(sizeof(Value));
        (*storedVal) = valdup(cadar);

        insertVectorItem(&(theEnv->names), i, (void *)strdup(caar.symbolValue));
        insertVectorItem(&(theEnv->values), i, (void *)storedVal);
        cc = cc->cdr.consValue;
        i++;
    }
    cc = bindings.consValue;
    i = 0;
    while(cc){
        ConsCell *binding = cc->car.consValue;
        Value caar = getNth(binding,0);
        Value cadar = getNth(binding,1);
        if( (caar.type == openType) || (cadar.type == openType) ){
            return false;
        }
        Value evaled = eval(cadar,theEnv);
        if( (evaled).type == openType){
            return false;
        }
        void *theEggman = NULL;
        getVectorItem(&(theEnv->values), i, &theEggman);
        *(Value *) theEggman = evaled;
        cc = cc->cdr.consValue;
        i++;
    }
    return true;
}

//Returns the nth item in the ConsCell* chain for convenience.
// This encodes errors as open parens.
Value getNth(ConsCell* cc, int n){
    if(cc == NULL){
        //This is an error.
        return evaluationError(NULL);
    }
    if(n>0){
        return getNth(cc->cdr.consValue, n-1);
    }else{
        return cc->car;
    }
}

// Return the value of a variable
Value resolveVar(Value expr, Frame *env){
    if (env == NULL){
        return evaluationError("symbol undefined in this context");
    }
    int i = 0;
    void *pointerToAChar = NULL; //RIP pointerToACharstar
    while(getVectorItem(&(env->names), i, &pointerToAChar)){
        if (strcmp( ((char *)pointerToAChar), expr.symbolValue) == 0){
            void *pointerToAValue = NULL;
            if(!getVectorItem(&(env->values), i, &pointerToAValue)){
                //This means we did frames wrong...
                return evaluationError("symbol was found, but does not correspond to a value");
            }
            //This casts the return value to pointer an dereferences it.
            Value ret = *((Value *)pointerToAValue);
            return ret;
        }
        i++;
    }
    return resolveVar(expr, env->parent);
}

/* Removes the Vectors and frees the Frame. */
void cleanupFrame(Frame *cleanMe){
    if(!cleanMe) return;
    void *pointerToAValue = NULL;
    int vectorSize = cleanMe->values.size;
    for(int i = 0; i < vectorSize; i++){
        getVectorItem(&(cleanMe->values),i,&pointerToAValue);
        freeValue(*((Value *)pointerToAValue));
        free((Value *)pointerToAValue);
    }

    void *pointerToAChar = NULL;
    vectorSize = cleanMe->names.size;
    for(int i = 0; i < vectorSize; i++){
        getVectorItem(&(cleanMe->names),i,&pointerToAChar);
        free((char *)pointerToAChar);
    }

    cleanupVector(&(cleanMe->names));
    cleanupVector(&(cleanMe->values));
    free(cleanMe);
}
