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
#include "pmath.h"
#include <curses.h>

void init_h(heap *h, int type) {
    h->size = 0;
    h->type = type;
}

void init_warr(heap *h, int type, process *arr, size_t arr_size) {
    h->size = (int)arr_size;
    h->type = type;
    for (int i=0; i<arr_size; i++) {
        h->data[i] = &arr[i];
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

void swap_p(process *arr[], int ix_a, int ix_b) {
    process *temp = arr[ix_b];
    arr[ix_b] = arr[ix_a];
    arr[ix_a] = temp;
    
}

int compare_priority(heap *h, int ix_a, int ix_b) {
    if (h->type == MIN_HEAP) {
        return h->data[ix_b]->priority - h->data[ix_a]->priority;
    } else if (h->type == MAX_HEAP) {
        return h->data[ix_a]->priority - h->data[ix_b]->priority;
    }
    return 0;
}

void sift_down(heap *h, int ix) {
    int curr_node = ix;
    int children = get_child(curr_node);
    while (children < h->size) {
        int max_child = h->type == MAX_HEAP ? max_ix_proc(h->data, children, children+1, h->size) : min_ix_proc(h->data, children, children+1, h->size);
        if(compare_priority(h, curr_node, max_child) < 0) {
            swap_p(h->data, curr_node, max_child);
            curr_node = max_child;
            children = get_child(curr_node);
            printw("SWAPPED -> ");
            for (int i=0; i<h->size; i++) {
                printw("(%d, %d)", h->data[i]->pid, h->data[i]->priority);
            }
            printw("\n");
            refresh();
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
            swap_p(h->data, curr_node, parent);
            curr_node = parent;
            continue;
        }
        break;
    }
}

void push(heap *h, process *p) {
    h->data[h->size] = p;
    h->size++;
    if(h->size > 1) {
        sift_up(h, h->size-1);
    }
}

process* pop(heap *h) {
    process *popped = h->data[0];
    clear();
    printw("PRE POP -> ");
    for (int i=0; i<h->size; i++) {
        printw("(%d, %d)", h->data[i]->pid, h->data[i]->priority);
    }
    printw("\n");
    h->data[0] = h->data[h->size-1];
//    h->data[h->size-1] = null_process();
    h->size--;
    printw("POST POP -> ");
    for (int i=0; i<h->size; i++) {
        printw("(%d, %d)", h->data[i]->pid, h->data[i]->priority);
    }
    printw("\n");
    if(h->size > 1) {
        sift_down(h, 0);
    }
    refresh();
    return popped;
}

process* peek(heap *h) {
    return h->data[0];
}

heap* heapify(process *arr, int type, size_t heap_size) {
    heap *h = malloc(sizeof(heap)+sizeof(arr));
    init_warr(h, type, arr, heap_size);
    for (int i=h->size; i>=0; i--) {
        sift_down(h, i);
    }
    for (int i=0; i<h->size; i++) {
        printw("(%d, %d)", h->data[i]->pid, h->data[i]->priority);
    }
    refresh();
    return h;
}
