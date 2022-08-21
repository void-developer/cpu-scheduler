//
//  heaps.h
//  cpu-scheduler
//
//  Created by Leonardo Angeli on 19/08/22.
//

#ifndef heaps_h
#define heaps_h

#include <stdio.h>
#include <stdbool.h>
#include "process.h"
#define MIN_HEAP 1
#define MAX_HEAP 0
typedef struct Heap {
    int size;
    int type;
    process *data[];
} heap;
typedef struct Heap heap;
bool is_heap_empty(heap *h);
void push(heap *h, process *p);
process* pop(heap *h);
heap* heapify(process arr[], int type, size_t heap_size);
#endif /* heaps_h */
