#ifndef __PRIMITIVES
#define __PRIMITIVES

#include <stdbool.h>
#include <string.h>
#include "common.h"
#include "interpreter.h"
#include "vector.h"

// The following assumes that the args vector is filled with the arguments <A,B,C...> of the appropriate type

// This does addition!... between the arguments in the vector args. It will accept any number of arguments
// It then returns the sum of all of the arguments (A+B+C...)
Value fn_plus(Vector args);

// Checks to see if a given list is null
Value fn_null(Vector args);

// checks to see if a given arg is a list
Value fn_list(Vector args);

// Returns the first element of the Quoted list args
Value fn_car(Vector args);

// Returns the entirety of a list with the exception of the first element
Value fn_cdr(Vector args);

// Creates a new list with a car that is arg1 and a cdr that is arg2
Value fn_cons(Vector args);

// Multiplies all of the numbers in the vector args. It will accept any number of arguments
// and then will return the product of all of the arguments
// A * B * C...
Value fn_times(Vector args);

// Subtracts the second number in the args vector from the first number in args and returns the result
// A - B
Value fn_minus(Vector args);

// Performs modulo division. Returns the remainder of integer division between the first and second args in the args vector
// A % B
Value fn_modulo(Vector args);

// Performs float division. If both args are ints and they divide evenly into one another it will return an int, else a float.
// It will divide the second argument by the first.
// A / B
Value fn_divide(Vector args);

//\\//\\ 
//The following methods accept two arguments and return true if the evaluation is successful and false otherwise
//\\//\\

// Checks to see if the first argument is less than or equal to the second argument
// A <= B
Value fn_lessTE(Vector args);

// Checks to see if the first argument is greater than or equal to the second argumet
// A >= B
Value fn_greaterTE(Vector args);

// Checks to see if the first argument is strictly less than the second
// A < B 
Value fn_lessT(Vector args);

// Checks to see if the first argument is stricly greater than the second
Value fn_greaterT(Vector args);

// Checks to see if the two arguments are numerically equal to one another
// A == B
Value fn_eqNum(Vector args);

// Ands all of the values in the vector args. It will accept any number of arguments
// and then will return false if all of the arguments evaluate to false, else true
// A && B && C...
Value fn_and(Vector args);

// Ors all of the values in the vector args. It will accept any number of arguments
// and then will return true if any of the arguments evaluate to true, else false
// A || B || C...
Value fn_or(Vector args);

// Returns the not of the the single argument in args
Value fn_not(Vector args);

//\\//\\
// End the Bool functions
//\\//\\

// Prints the first item in the vector provided it is of string, real or int type
Value fn_display(Vector args);

// Finds the length of the string that is given to it as the first element in the vector.
// Returns an int
Value fn_stringLength(Vector args);

// Checks to see if the two items are strucurally equal to each other
// A.equals(B)
Value fn_equal(Vector args);

// Adds the primitive functions to the frame (call only on the top level env).
void addPrimitives(Frame *frame);

#endif
