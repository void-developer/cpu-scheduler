//
//  heaps.c
//  cpu-scheduler
//
//  Created by Leonardo Angeli on 19/08/22.
//

#include "heaps.h"
#include "process.h"
#include <stdbool.h>

struct Heap {
    int size;
    process data[];
};

typedef struct Heap heap;

bool is_heap_empty(heap *h) {
    return h->size == 0;
}

void push(heap *h, process *p) {
    h->data[h->size] = *p;
    h->size++;
    
}

