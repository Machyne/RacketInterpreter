#ifndef __INTERPRETER
#define __INTERPRETER

#include <stdbool.h>
#include <string.h>
#include "common.h"
#include "vector.h"
#include "escape.h"

// Frames have a list of names, a list of values and a poitner to their parent frame.
// The names and values are indexed so that they refer to the same variable

struct __Frame
{
    struct __Frame *parent;
    Vector names;
    Vector values;
};

struct __Closure {
    struct __Frame *env;
    Value formals;
    Value body;
};

typedef struct __Closure Closure;
typedef struct __Frame Frame;

// Returns a value of type void.
Value voidVal();

//Prints the error message and returns an error-denoting open type.
Value evaluationError(char *errorMessage);

/* Removes the Vectors and frees the Frame. */
void cleanupFrame(Frame *frame);

// Evaluates the expression. In the frame that it is given
Value eval(Value expr, Frame *env);

// Evaluates an If expression
Value evalIf(Value tree, Frame *env);

// Evaluates a Quote expression
Value evalQuote(Value tree, Frame *env);

// Evaluates a let expression and makes a new frame that will point to the frame that is given to it
// as a parameter
Value evalLet(Value tree, Frame *env);

// Evaluates a let* expression and makes a new frame that will point to the frame that is given to it
// as a parameter
Value evalLetStar(Value tree, Frame *env);

// Evaluates a letRec expression and makes a new frame that will point to the frame that is given to it
// as a parameter
Value evalLetRec(Value tree, Frame *env);

// Evaluates a define expression and makes a new frame that will point to the frame that is given to it
// as a parameter
Value evalDefine(Value tree, Frame *env);

// Evaluates a lambda statement and returns a new closure.
Value evalLambda(Value tree, Frame *env);

// Evaluates a begin statement.
Value evalBegin(Value tree, Frame *env);

// Evaluates a cond expression. Evaluates the cadr of the first thing with a car that evaluates to true
Value evalCond(Value tree, Frame *env);

// Returns the nth item in the ConsCell* chain for convenience.
// This has no error checking, be careful not to fall off the end!
Value getNth(ConsCell* cc, int n);

// Return the value of a variable
Value resolveVar(Value expr, Frame *env);

// Checks to see if the value is in the environment or its parents
// Returns a value with type = openType if this is not found.
Value getVal(Value expr, Frame *env);

// Makes a new frame that points to its parent
// uses a Frame* so that the first one can point to NULL and it is prettier
Frame* makeFrame(Frame *parent);

// Used in conjunction with makeFrame. Adds bindings to the Frame theEnv from the Value bindings
// bindings is a linked list of pairs
// each pair has a name and a value, with the name first followed immediately by the value
bool addBindings(Frame *theEnv, Value bindings);

// Adds bindings to a frame. Used in let*
bool addBindingsStar(Frame *theEnv, Value bindings);

// Adds bindings to a frame. Used in letrec
bool addBindingsRec(Frame *theEnv, Value bindings);

// Duplicates a given given value (recursively) and heap allocates the pointers to the duplicate(s)
Value valdup(Value val);

// allows us to evaluate user defined functions
Value evalApply(Value expr, Frame *env);

// Allows us to apply userdefined functions
Value apply(Value function, Vector args);

// changes a given value to it "quoted" counterpart
Value quote(Value toQuote);

// changes "quoted" value to its unquoted counterpart
Value unquote(Value quoted);


#endif
