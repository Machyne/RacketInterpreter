/*| Submitted by:
|*| Matt Cotter (Not here to object to not object to being one of the walrii)
|*| Greg Fournier (walrii)
|*| Joy Hill (walrii)
|*/

#include "primitives.h"
#include "interpreter.h"
#include "parstack.h"
#include <stdlib.h>
#include <math.h>
#include <stdio.h>

extern Vector *consGarbage;



// Adds the primitive functions to the frame (call only on the top level env).
void addPrimitives(Frame *frame){
    //---------------  PLUS  ---------------
    Value *plus = (Value *)malloc(sizeof(Value));
    plus->type = primitiveType;
    plus->primitiveValue = fn_plus;
    insertVectorItem(&(frame->names), 0, (void *)strdup("+"));
    insertVectorItem(&(frame->values), 0, (void *)plus);
    //--------------------------------------

    //---------------  NULL?  ---------------
    Value *null = (Value *)malloc(sizeof(Value));
    null->type = primitiveType;
    null->primitiveValue = fn_null;
    insertVectorItem(&(frame->names), 0, (void *)strdup("null?"));
    insertVectorItem(&(frame->values), 0, (void *)null);
    //--------------------------------------

    //---------------  LIST?  ---------------
    Value *list = (Value *)malloc(sizeof(Value));
    list->type = primitiveType;
    list->primitiveValue = fn_list;
    insertVectorItem(&(frame->names), 0, (void *)strdup("list?"));
    insertVectorItem(&(frame->values), 0, (void *)list);
    //--------------------------------------

    //---------------  CAR  ---------------
    Value *car = (Value *)malloc(sizeof(Value));
    car->type = primitiveType;
    car->primitiveValue = fn_car;
    insertVectorItem(&(frame->names), 0, (void *)strdup("car"));
    insertVectorItem(&(frame->values), 0, (void *)car);
    //--------------------------------------

    //---------------  CDR  ---------------
    Value *cdr = (Value *)malloc(sizeof(Value));
    cdr->type = primitiveType;
    cdr->primitiveValue = fn_cdr;
    insertVectorItem(&(frame->names), 0, (void *)strdup("cdr"));
    insertVectorItem(&(frame->values), 0, (void *)cdr);
    //--------------------------------------

    //---------------  CONS  ---------------
    Value *cons = (Value *)malloc(sizeof(Value));
    cons->type = primitiveType;
    cons->primitiveValue = fn_cons;
    insertVectorItem(&(frame->names), 0, (void *)strdup("cons"));
    insertVectorItem(&(frame->values), 0, (void *)cons);
    //--------------------------------------

    //---------------  TIMES  ---------------
    Value *times = (Value *)malloc(sizeof(Value));
    times->type = primitiveType;
    times->primitiveValue = fn_times;
    insertVectorItem(&(frame->names), 0, (void *)strdup("*"));
    insertVectorItem(&(frame->values), 0, (void *)times);
    //--------------------------------------

    //---------------  MINUS  ---------------
    Value *minus = (Value *)malloc(sizeof(Value));
    minus->type = primitiveType;
    minus->primitiveValue = fn_minus;
    insertVectorItem(&(frame->names), 0, (void *)strdup("-"));
    insertVectorItem(&(frame->values), 0, (void *)minus);
    //--------------------------------------

    //---------------  MODULO  ---------------
    Value *modulo = (Value *)malloc(sizeof(Value));
    modulo->type = primitiveType;
    modulo->primitiveValue = fn_modulo;
    insertVectorItem(&(frame->names), 0, (void *)strdup("modulo"));
    insertVectorItem(&(frame->values), 0, (void *)modulo);
    //--------------------------------------

    //---------------  DIVIDE  ---------------
    Value *divide = (Value *)malloc(sizeof(Value));
    divide->type = primitiveType;
    divide->primitiveValue = fn_divide;
    insertVectorItem(&(frame->names), 0, (void *)strdup("/"));
    insertVectorItem(&(frame->values), 0, (void *)divide);
    //--------------------------------------

    //---------------  LTE  ---------------
    Value *lessTE = (Value *)malloc(sizeof(Value));
    lessTE->type = primitiveType;
    lessTE->primitiveValue = fn_lessTE;
    insertVectorItem(&(frame->names), 0, (void *)strdup("<="));
    insertVectorItem(&(frame->values), 0, (void *)lessTE);
    //--------------------------------------

    //---------------  GTE  ---------------
    Value *greaterTE = (Value *)malloc(sizeof(Value));
    greaterTE->type = primitiveType;
    greaterTE->primitiveValue = fn_greaterTE;
    insertVectorItem(&(frame->names), 0, (void *)strdup(">="));
    insertVectorItem(&(frame->values), 0, (void *)greaterTE);
    //--------------------------------------

    //---------------  LESS  ---------------
    Value *lessT = (Value *)malloc(sizeof(Value));
    lessT->type = primitiveType;
    lessT->primitiveValue = fn_lessT;
    insertVectorItem(&(frame->names), 0, (void *)strdup("<"));
    insertVectorItem(&(frame->values), 0, (void *)lessT);
    //--------------------------------------

    //---------------  GREATER  ---------------
    Value *greaterT = (Value *)malloc(sizeof(Value));
    greaterT->type = primitiveType;
    greaterT->primitiveValue = fn_greaterT;
    insertVectorItem(&(frame->names), 0, (void *)strdup(">"));
    insertVectorItem(&(frame->values), 0, (void *)greaterT);
    //--------------------------------------

    //---------------  AND  ---------------
    Value *and = (Value *)malloc(sizeof(Value));
    and->type = primitiveType;
    and->primitiveValue = fn_and;
    insertVectorItem(&(frame->names), 0, (void *)strdup("and"));
    insertVectorItem(&(frame->values), 0, (void *)and);
    //--------------------------------------

    //---------------  OR  ---------------
    Value *or = (Value *)malloc(sizeof(Value));
    or->type = primitiveType;
    or->primitiveValue = fn_or;
    insertVectorItem(&(frame->names), 0, (void *)strdup("or"));
    insertVectorItem(&(frame->values), 0, (void *)or);
    //--------------------------------------
    
    //---------------  NOT  ---------------
    Value *not = (Value *)malloc(sizeof(Value));
    not->type = primitiveType;
    not->primitiveValue = fn_not;
    insertVectorItem(&(frame->names), 0, (void *)strdup("not"));
    insertVectorItem(&(frame->values), 0, (void *)not);
    //--------------------------------------

    //---------------  NUMERIC EQUALITY  ---------------
    Value *eqNum = (Value *)malloc(sizeof(Value));
    eqNum->type = primitiveType;
    eqNum->primitiveValue = fn_eqNum;
    insertVectorItem(&(frame->names), 0, (void *)strdup("="));
    insertVectorItem(&(frame->values), 0, (void *)eqNum);
    //--------------------------------------

    //---------------  STRUCTURAL EQUALITY  ---------------
    Value *equal = (Value *)malloc(sizeof(Value));
    equal->type = primitiveType;
    equal->primitiveValue = fn_equal;
    insertVectorItem(&(frame->names), 0, (void *)strdup("equal?"));
    insertVectorItem(&(frame->values), 0, (void *)equal);
    //--------------------------------------

    //---------------  DISPLAY  ---------------
    Value *display = (Value *)malloc(sizeof(Value));
    display->type = primitiveType;
    display->primitiveValue = fn_display;
    insertVectorItem(&(frame->names), 0, (void *)strdup("display"));
    insertVectorItem(&(frame->values), 0, (void *)display);
    //--------------------------------------

    //---------------  STRING-LENGTH  ---------------
    Value *stringLength = (Value *)malloc(sizeof(Value));
    stringLength->type = primitiveType;
    stringLength->primitiveValue = fn_stringLength;
    insertVectorItem(&(frame->names), 0, (void *)strdup("string-length"));
    insertVectorItem(&(frame->values), 0, (void *)stringLength);
    //--------------------------------------
}

// gets the ith argument from the args vector
Value getArg(Vector args, int i){
    void *pointerToAValue = NULL;
    getVectorItem(&args, i-1, &pointerToAValue);
    return (*((Value *)pointerToAValue));
}


// This does addition!... between the arguments in the vector args. It will accept any number of arguments
// It then returns the sum of all of the arguments
Value fn_plus(Vector args){
    int acc1 = 0;
    float acc2 = 0.0f;
    bool isFloat = false;

    //sum values
    for(int i = 0; i < args.size; i++){
       Value arg = getArg(args, i+1);
       //keep track of int sum
       if (arg.type == realType) {
         isFloat = true;
         acc2 += arg.realValue;
       }else if (arg.type == integerType) {
         acc1 += arg.integerValue;
         acc2 += arg.integerValue;
       }else{
         return evaluationError("non number type given to a \'+\'");
       }
    }

    //create return
    Value ret;
    if(isFloat){
       ret.type = realType;
        ret.realValue = acc2;
    }else{
       ret.type = integerType;
        ret.integerValue = acc1;
    }

    return ret;
}

// checks to see if a given list is null
Value fn_null(Vector args){
    if(args.size != 1){
       return evaluationError("wrong number of arguments given to a \'null?\'");
    }
    Value arg = getArg(args, 1);

    Value ret;
    ret.type = booleanType;
    ret.booleanValue = false;

    if (arg.type == quotedConsType) {
        ret.booleanValue = (arg.consValue == NULL);
    }

    return ret;
}

// checks to see if a given arg is a list
Value fn_list(Vector args){
    if(args.size != 1){
       return evaluationError("wrong number of arguments given to a \'null?\'");
    }
    Value arg = getArg(args, 1);

    Value ret;
    ret.type = booleanType;
    ret.booleanValue = (arg.type == quotedConsType);
    return ret;
}

// Returns the first element of the Quoted list args
Value fn_car(Vector args){
    if(args.size != 1){
       return evaluationError("wrong number of arguments given to a \'car\'");
    }
    
    Value arg = getArg(args, 1);
    if( arg.type != quotedConsType ){
       return evaluationError("non quoted-cons type given to a \'car\'");
    }

    return quote(arg.consValue->car);
}

// returns the entirety of a list with the exception of the first element
Value fn_cdr(Vector args){
    if(args.size != 1){
       return evaluationError("wrong number of arguments given to a \'cdr\'");
    }

    Value arg = getArg(args, 1);
    if( arg.type != quotedConsType ){
       return evaluationError("non quoted-cons type given to a \'cdr\'");
    }

    return quote(arg.consValue->cdr);
}

// combines two lists
Value fn_cons(Vector args){
    if(args.size != 2){
       return evaluationError("wrong number of arguments given to a \'cons\'");
    }

    Value arg1 = unquote(getArg(args, 1));
    Value arg2 = unquote(getArg(args, 2));
    
    if( (arg1.type == openType) || (arg2.type == openType) ){
       return evaluationError("non quoted typed given to a cons");
    }

    Value ret;
    ret.type = quotedConsType;
    ConsCell *cc = (ConsCell *) malloc(sizeof(ConsCell));
    cc->car = arg1;
    cc->cdr = arg2;
    ret.consValue = cc;

    insertVectorItem(consGarbage, consGarbage->size, (void*)cc);

    return ret;
}

// Multiplies all of the numbers in the vector args. It will accept any number of arguments
// and then will return the product of all of the arguments
// A * B * C... 
Value fn_times(Vector args){
    int acc1 = 1;
    float acc2 = 1.0f;
    bool isFloat = false;

    //sum values
    for(int i = 0; i < args.size; i++){
       Value arg = getArg(args, i+1);
       //keep track of int sum
       if (arg.type == realType) {
         isFloat = true;
         acc2 *= arg.realValue;
       }else if (arg.type == integerType) {
         acc1 *= arg.integerValue;
         acc2 *= arg.integerValue;
       }else{
         return evaluationError("non number type given to a \'*\'");
       }
    }

    //create return
    Value ret;
    if(isFloat){
       ret.type = realType;
        ret.realValue = acc2;
    }else{
       ret.type = integerType;
        ret.integerValue = acc1;
    }

    return ret;
}

// Subtracts the second number in the args vector from the first number in args and returns the result
// A - B 
Value fn_minus(Vector args){
    if(args.size != 2){
       return evaluationError("wrong number of arguments given to a \'-\'");
    }

    Value arg1 = getArg(args, 1);
    Value arg2 = getArg(args, 2);
    
    if( (arg1.type != realType) && (arg1.type != integerType) ){
       return evaluationError("first argument given to a \'-\' was not of number type");
    }

    if( (arg2.type != realType) && (arg2.type != integerType) ){
       return evaluationError("second argument given to a \'-\' was not of number type");
    }

    Value ret;
    if( (arg1.type == realType) || (arg2.type == realType) ){
       ret.type = realType;
       ret.realValue = arg1.realValue - arg2.realValue;
    }else{
       ret.type = integerType;
       ret.integerValue = arg1.integerValue - arg2.integerValue;
    }
    
    return ret;
}

// Performs modulo division. Returns the remainder of integer division between the first and second args in the args vector
// A % B
Value fn_modulo(Vector args){
    if(args.size != 2){
       return evaluationError("wrong number of arguments given to a \'modulo\'");
    }

    Value arg1 = getArg(args, 1);
    Value arg2 = getArg(args, 2);
    
    if( arg1.type != integerType ){
       return evaluationError("first argument given to a \'modulo\' was not of integer type");
    }

    if( arg2.type != integerType ){
       return evaluationError("second argument given to a \'modulo\' was not of integer type");
    }

    Value ret;
    ret.type = integerType;
    ret.integerValue = (arg1.integerValue % arg2.integerValue);
    
    return ret;
}

// Performs float division. If both args are ints and they divide evenly into one another it will return an int, else a float.
// It will divide the second argument by the first.
// A / B
Value fn_divide(Vector args){
    if(args.size != 2){
       return evaluationError("wrong number of arguments given to a \'/\'");
    }

    Value arg1 = getArg(args, 1);
    Value arg2 = getArg(args, 2);
    
    if( (arg1.type != realType) && (arg1.type != integerType) ){
       return evaluationError("first argument given to a \'/\' was not of number type");
    }

    if( (arg2.type != realType) && (arg2.type != integerType) ){
       return evaluationError("second argument given to a \'/\' was not of number type");
    }

    Value ret;
    if( (arg1.type == realType) && (arg2.type == realType) ){
       ret.type = realType;
       ret.realValue = arg1.realValue / arg2.realValue;
    }else if( (arg1.type == integerType) && (arg2.type == realType) ){
       ret.type = realType;
       ret.realValue = ((float)arg1.integerValue) / arg2.realValue;
    }else if( (arg1.type == realType) && (arg2.type == integerType) ){
       ret.type = realType;
       ret.realValue = arg1.realValue / ((float)arg2.integerValue);
    }else if(arg1.integerValue % arg2.integerValue != 0){
       ret.type = realType;
       ret.realValue = ((float)arg1.integerValue) / ((float)arg2.integerValue);
    }else{
       ret.type = integerType;
       ret.integerValue = arg1.integerValue / arg2.integerValue;
    }
    
    return ret;
}

// Gets the difference between the two arguments
float getDif(Value a, Value b){
    float f;
    if( (a.type == realType) && (b.type == realType) ){
       f = a.realValue - b.realValue;
    }else if( (a.type == integerType) && (b.type == realType) ){
       f = ((float)a.integerValue) - b.realValue;
    }else if( (a.type == realType) && (b.type == integerType) ){
       f = a.realValue - ((float)b.integerValue);
    }else {
       f = ((float)a.integerValue) - ((float)b.integerValue);
    }
    return f;
}

// Checks to see if the first argument is less than or equal to the second argument
// A <= B
Value fn_lessTE(Vector args){
    if(args.size != 2){
       return evaluationError("wrong number of arguments given to a \'<=\'");
    }

    Value arg1 = getArg(args, 1);
    Value arg2 = getArg(args, 2);
    
    if( (arg1.type != realType) && (arg1.type != integerType) ){
       return evaluationError("first argument given to a \'<=\' was not of number type");
    }

    if( (arg2.type != realType) && (arg2.type != integerType) ){
       return evaluationError("second argument given to a \'<=\' was not of number type");
    }

    Value ret;
    ret.type = booleanType;
    ret.booleanValue = (getDif(arg1, arg2) <= 0.0f);
    
    return ret;
}

// Checks to see if the first argument is greater than or equal to the second argumet
// A >= B
Value fn_greaterTE(Vector args){
    if(args.size != 2){
       return evaluationError("wrong number of arguments given to a \'>=\'");
    }

    Value arg1 = getArg(args, 1);
    Value arg2 = getArg(args, 2);
    
    if( (arg1.type != realType) && (arg1.type != integerType) ){
       return evaluationError("first argument given to a \'>=\' was not of number type");
    }

    if( (arg2.type != realType) && (arg2.type != integerType) ){
       return evaluationError("second argument given to a \'>=\' was not of number type");
    }

    Value ret;
    ret.type = booleanType;
    ret.booleanValue = (getDif(arg1, arg2) >= 0.0f);
    
    return ret;
}

// Checks to see if the first argument is strictly less than the second
// A < B
Value fn_lessT(Vector args){
    if(args.size != 2){
       return evaluationError("wrong number of arguments given to a \'<\'");
    }

    Value arg1 = getArg(args, 1);
    Value arg2 = getArg(args, 2);
    
    if( (arg1.type != realType) && (arg1.type != integerType) ){
       return evaluationError("first argument given to a \'<\' was not of number type");
    }

    if( (arg2.type != realType) && (arg2.type != integerType) ){
       return evaluationError("second argument given to a \'<\' was not of number type");
    }

    Value ret;
    ret.type = booleanType;
    ret.booleanValue = (getDif(arg1, arg2) < 0.0f);
    
    return ret;
}

// Checks to see if the first argument is stricly greater than the second
Value fn_greaterT(Vector args){
    if(args.size != 2){
       return evaluationError("wrong number of arguments given to a \'>\'");
    }

    Value arg1 = getArg(args, 1);
    Value arg2 = getArg(args, 2);
    
    if( (arg1.type != realType) && (arg1.type != integerType) ){
       return evaluationError("first argument given to a \'>\' was not of number type");
    }

    if( (arg2.type != realType) && (arg2.type != integerType) ){
       return evaluationError("second argument given to a \'>\' was not of number type");
    }

    Value ret;
    ret.type = booleanType;
    ret.booleanValue = (getDif(arg1, arg2) > 0.0f);
    
    return ret;
}

// Ands all of the values in the vector args. It will accept any number of arguments
// and then will return false if any of the arguments evaluate to false, else last arg
// A && B && C...
Value fn_and(Vector args){
    //create return
    Value ret;
    ret.type = booleanType;
    ret.booleanValue = true;

    //and values
    for(int i = 0; i < args.size; i++){
       Value arg = getArg(args, i+1);
       if ( (arg.type == booleanType) && !arg.booleanValue) {
         ret.type = booleanType;
         ret.booleanValue = false;
         return ret;
       }
       ret = arg;
    }
    return ret;
}

// Ors all of the values in the vector args. It will accept any number of arguments
// and then will return the first non-false argument, else false
// A || B || C...
Value fn_or(Vector args){
    //and values
    for(int i = 0; i < args.size; i++){
       Value arg = getArg(args, i+1);
       if ( (arg.type != booleanType) || arg.booleanValue) {
         return arg;
       }
    }

    //create return
    Value ret;
    ret.type = booleanType;
    ret.booleanValue = false;
    return ret;
}

// Returns the not of the single argument in args
Value fn_not(Vector args){
    // error check
    Value toBeNotted = getArg(args,0);
    if(args.size != 1){
       return evaluationError("Too many arguments for a \'not\' function");
    }
    if(toBeNotted.type != booleanType){
       return evaluationError("Tried to \'not\' a non-boolean");
    }
    // switch bool values
    
    toBeNotted.booleanValue = !toBeNotted.booleanValue;
    return toBeNotted;
}

// Checks to see if the two arguments are numerically equal to one another
// A == B
Value fn_eqNum(Vector args){
    if(args.size != 2){
       return evaluationError("wrong number of arguments given to a \'=\'");
    }

    Value arg1 = getArg(args, 1);
    Value arg2 = getArg(args, 2);
    
    if( (arg1.type != realType) && (arg1.type != integerType) ){
       return evaluationError("first argument given to a \'=\' was not of number type");
    }

    if( (arg2.type != realType) && (arg2.type != integerType) ){
       return evaluationError("second argument given to a \'=\' was not of number type");
    }

    Value ret;
    ret.type = booleanType;
    ret.booleanValue = (getDif(arg1, arg2) == 0.0f);
    
    return ret;
}

// Prints the first item in the vector provided it is of string, real or int type
Value fn_display(Vector args){
    if(args.size != 1){
       return evaluationError("wrong number of arguments given to a \'display\'");
    }

    Value arg = getArg(args, 1);
    if( arg.type == stringType ){
       char* toPrint = unescape(arg.stringValue);
       printf("%s", toPrint);
       free(toPrint);
    }else if( (arg.type == realType) || (arg.type == integerType) ){
       printParseTree(arg);
    }else{
       return evaluationError("bad argument given to \'display\'");
    }

    return voidVal();
}

// Finds the length of the string that is given to it as the first element in the vector.
// Returns an int
Value fn_stringLength(Vector args){
    if(args.size != 1){
       return evaluationError("wrong number of arguments given to a \'string-length\'");
    }

    Value arg = getArg(args, 1);
    if( arg.type == stringType ){
       Value ret;
       ret.type = integerType;
       ret.integerValue = unescape_counter(arg.stringValue);
       return ret;
    }

    return evaluationError("argument given to \'string-length\'"); 
}

// Checks structural equality of two values
bool isEqual(Value a, Value b){
    if(b.type != a.type) return false;
    switch(a.type){
        case(booleanType):
            return a.booleanValue == b.booleanValue;
        case(integerType):
            return a.integerValue == b.integerValue;
        case(realType):
            return a.realValue == b.realValue;
        case(stringType):
            return (strcmp(a.stringValue, b.stringValue) == 0);
        case(quotedSymbolType):
        case(symbolType):
            return (strcmp(a.symbolValue, b.symbolValue) == 0);
        case(closureType):
           return a.closureValue == b.closureValue;
        case(primitiveType):
            return a.primitiveValue == b.primitiveValue;
        case(quotedConsType):
        case(consType):
            if(!(a.consValue && b.consValue)){
                return true;
            }else if((!a.consValue) || (!b.consValue)){
                return false;
            }
            return (isEqual(a.consValue->car, b.consValue->car) &&
               isEqual(a.consValue->cdr, b.consValue->cdr));
        default:
            break;
    }
    return false;
}

// Checks to see if the two items are strucurally equal to each other
// A.equals(B)
Value fn_equal(Vector args){
    if(args.size != 2){
       return evaluationError("wrong number of arguments given to a \'equal?\'");
    }

    Value arg1 = getArg(args, 1);
    Value arg2 = getArg(args, 2);
    


    Value ret;
    ret.type = booleanType;
    ret.booleanValue = isEqual(arg1, arg2);

    return ret;
}
