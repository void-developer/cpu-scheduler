//
//  pmath.c
//  cpu-scheduler
//
//  Created by Leonardo Angeli on 20/08/22.
//

#include "pmath.h"
#include "process.h"
/**
 Returns the maximum of two integers
 @param a first integer
 @param b first integer
 */
int max(int a, int b) {
    if(a > b) {
        return a;
    }
    return b;
}

/**
 Returns between the two indexes given as input, the one pointing to the maximum of the two elements in the array
 @param arr the array of which elements to check
 @param ix_a the index of the first element to compare
 @param ix_b the index of the second element to compare
 */
int max_ix(int arr[], int ix_a, int ix_b) {
    if(arr[ix_a] > arr[ix_b]) {
        return ix_a;
    }
    return ix_b;
}

/**
 Returns between the two indexes given as input, the one pointing to the maximum of the two processes (comparing the priority) in the array
 @param arr the array of which elements to check
 @param ix_a the index of the first element to compare
 @param ix_b the index of the second element to compare
 */
int max_ix_proc(process *arr[], int ix_a, int ix_b, int size) {
    if(ix_a < size && (ix_b >= size || arr[ix_a]->priority > arr[ix_b]->priority)) {
        return ix_a;
    }
    return ix_b;
}

/**
 Returns between the two indexes given as input, the one pointing to the minimum of the two processes (comparing the priority) in the array
 @param arr the array of which elements to check
 @param ix_a the index of the first element to compare
 @param ix_b the index of the second element to compare
 */
int min_ix_proc(process *arr[], int ix_a, int ix_b, int size) {
    if(ix_a < size && (ix_b >= size || arr[ix_a]->priority < arr[ix_b]->priority)) {
        return ix_a;
    }
    return ix_b;
}
