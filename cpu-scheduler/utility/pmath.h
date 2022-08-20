//
//  pmath.h
//  cpu-scheduler
//
//  Created by Leonardo Angeli on 20/08/22.
//

#ifndef pmath_h
#define pmath_h

#include <stdio.h>
#include "../process.h"
int max(int a, int b);
int max_ix(int arr[], int ix_a, int ix_b);
int max_ix_proc(process arr[], int ix_a, int ix_b);
int min_ix_proc(process arr[], int ix_a, int ix_b);
#endif /* pmath_h */
