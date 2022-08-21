//
//  parrays.c
//  cpu-scheduler
//
//  Created by Leonardo Angeli on 20/08/22.
//

#include "parrays.h"

/**
 Swaps the address of two elements
 @param a address of first element
 @param b address of second element
 */
void swap(int *a, int *b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}
