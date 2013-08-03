/*
 * Submission by:
 *   Matt Cotter
 *   Greg Fournier
 */

#include "vector.h"
#include <stdio.h>
#include <stdlib.h>

/* Take an already existing Vector as a parameter. Set its internal memory
   size to match the value given in the parameter, allocate an 
   appropriately-sized C array to hold the actual data and initialize the 
   size variable to 0. 
*/
void initVector(Vector *vector, int memorySize){
    vector->memorySize = memorySize;
    vector->size = 0;
    vector->array = (void **) malloc(memorySize);
}

/*  Removes the array associated with the Vector. */
void cleanupVector(Vector *vector){
    free(vector->array);
    vector->memorySize = 0;
    vector->size = 0;
}

/* Print out the vector for debugging purposes. */
void printVector(Vector *vector){
    // Empty(null) case
    if(vector->size == 0){
       printf("< >\n");
       return;
    }

    // The sort of vector that has elements
    printf("<");
    for (int i = 0; i < vector->size-1; ++i)
    {
       printf("%p, ", vector->array[i]);
    }

    printf("%p>\n", vector->array[vector->size-1]);
}

/*  Insert value at location inside the Vector. If there is already
    data at that location, insert slides the rest of the data down one
    position to make room for the new value. Returns 1 if success, and
    0 if the location is invalid (less than 0, or greater than the
    size). Inserting at the very end of the current data (at position
    equal to size) is fine, and should increase size by 1. If the internal 
    array is too small to fit a new value, create a new array of twice 
    the size using realloc. 
*/
int insertVectorItem(Vector *vector, int location, void *value){
    if(location < 0 || location > vector->size){
       return 0;
    }
    vector->size++;

    //In this case, the vector size needs to be doubled.
    if ( (vector->memorySize) < (sizeof(void*) * (vector->size)) ){
       vector->array = realloc(vector->array, 2 * vector->memorySize);
       vector->memorySize *= 2;
    }

    for (int i = vector->size - 1; i > location; i--){
       vector->array[i] = vector->array[i - 1];
    }
    vector->array[location] = value;
    
    return 1;
}

/*  Obtains value at location inside the Vector. Returns 1 if success,
    and 0 if the location is invalid (less than 0, or greater than or
    equal to the size). The value itself is returned through the
    parameter value. 
*/
int getVectorItem(Vector *vector, int location, void **value){
    if(location < 0 || location >= vector->size){
       return 0;
       printf("Invalid location\n");
    }

    *value = vector->array[location];

    return 1;
}

/*  Deletes value at location inside the Vector.  Returns 1 if success,
    and 0 if the location is invalid (less than 0, or greater than or
    equal to the size). When an item is deleted, everything else past
    it in the array should be slid down.
*/
int deleteVectorItem(Vector *vector, int location){
    if((location < 0) || (location >= vector->size)){
       return 0;
    }

    vector->size--;
    for (int i = location; i < vector->size; i++){
       vector->array[i] = vector->array[i + 1];
    }
    vector->array[vector->size]=NULL;
    return 1;
}
