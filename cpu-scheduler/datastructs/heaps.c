//
//  heaps.c
//  cpu-scheduler
//
//  Created by Leonardo Angeli on 19/08/22.
//

#include "heaps.h"
#include "process.h"
#include <stdio.h>
#include "stdlib.h"
#include <stdbool.h>
#include "../utility/pmath.h"

void init_h(heap *h, int type) {
    h->size = 0;
    h->type = type;
    for (int i=0; i<(int)(sizeof(*h->data)/sizeof(process)); i++) {
        h->data[i] = null_process();
    }
}

bool is_heap_empty(heap *h) {
    return h->size == 0;
}

int get_child(int ix) {
    return (ix*2)+1;
}

int get_parent(int ix) {
    if(ix==0) {
        return ix;
    }
    return (ix/2)-1;
}

void swap_p(process *pa, process *pb) {
    process temp = *pa;
    *pb = *pa;
    *pa = temp;
    
}

int compare_priority(heap *h, int ix_a, int ix_b) {
    if (h->type == MIN_HEAP) {
        return h->data[ix_b].priority - h->data[ix_a].priority;
    } else if (h->type == MAX_HEAP) {
        return h->data[ix_a].priority - h->data[ix_b].priority;
    }
    return 0;
}

void sift_down(heap *h, int ix) {
    int curr_node = ix;
    int children = get_child(curr_node);
    while (children > h->size) {
        int max_child = h->type == MAX_HEAP ? max_ix_proc(h->data, children, children+1) : min_ix_proc(h->data, children, children+1);
        if(compare_priority(h, curr_node, max_child) < 0) {
            swap_p(&h->data[curr_node], &h->data[max_child]);
            children = get_child(curr_node);
            continue;
        }
        break;
    }
}

void sift_up(heap *h, int ix) {
    int curr_node = ix;
    while(curr_node > 0) {
        int parent = get_parent(curr_node);
        if(compare_priority(h, curr_node, parent) > 0) {
            swap_p(&h->data[curr_node], &h->data[parent]);
            continue;
        }
        break;
    }
}

void push(heap *h, process *p) {
    h->data[h->size] = *p;
    h->size++;
    if(h->size > 1) {
        sift_up(h, h->size-1);
    }
}

process* pop(heap *h) {
    process *popped = &h->data[0];
    h->data[0] = h->data[h->size];
    h->data[h->size] = null_process();
    h->size--;
    if(h->size > 1) {
        sift_down(h, 0);
    }
    return popped;
}

process* peek(heap *h) {
    return &h->data[0];
}

heap* heapify(process *arr, int type) {
    heap *h = malloc(sizeof(heap)+sizeof(arr));
    init_h(h, type);
    for (int i=h->size; i>=0; i--) {
        sift_down(h, i);
    }
    return h;
}
