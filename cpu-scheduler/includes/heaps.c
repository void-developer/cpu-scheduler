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

/**
 Initializes the heap with the default parameters and the given type. The initial size of the heap is 0.
 The type can be either 0 or 1 which defines whether the heap is a max heap (0) or a min heap (1). Both values are defined as MAX_HEAP and MIN_HEAP respectively
 @param h non initialized heap
 @param type the heap's type
*/
void init_h(heap *h, int type) {
    h->size = 0;
    h->type = type;
}

/**
 Initializes the heap with the given array. The resulting heap will have a pre-filled array with pointers to the elements of the given array.
 The type can be either 0 or 1 which defines whether the heap is a max heap (0) or a min heap (1). Both values are defined as MAX_HEAP and MIN_HEAP respectively
 @param h heap
 @param type the heap's type
 @param arr the array to use for the heap
 @param arr_size the size of the given array
*/
void init_warr(heap *h, int type, process *arr, size_t arr_size) {
    h->size = (int)arr_size;
    h->type = type;
    for (int i=0; i<arr_size; i++) {
        h->data[i] = &arr[i];
    }
}

/**
 Checks whether the heap's empty or not
 @param h heap
 @return true if empty, false otherwise
 */
bool is_heap_empty(heap *h) {
    return h->size == 0;
}

/**
 Returns the left most child's index of a heap element
 @param ix parent's index
 @return leftmost child's index
 */
int get_child(int ix) {
    return (ix*2)+1;
}

/**
 Returns the given heap element's parent
 @param ix child's index
 @return parent's index
 */
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

/**
 Compares the priority of two heap elements based on the type of heap. If the first element has more priority the function returns a value > 0, < 0 otherwise. If they have the same priority the function returns 0.
 @param h heap
 @param ix_a the first element to compare
 @param ix_b the second element to compare
 @return difference between the two priorities
 */
int compare_priority(heap *h, int ix_a, int ix_b) {
    if (h->type == MIN_HEAP) {
        return h->data[ix_b]->priority - h->data[ix_a]->priority;
    } else if (h->type == MAX_HEAP) {
        return h->data[ix_a]->priority - h->data[ix_b]->priority;
    }
    return 0;
}
/**
 Re-establishes the given heap node balanced position (if it is in a wrong one) by shifting it down until both of his children have less priority than it. This operation takes O(logN) time to perform, which is more efficient than a simple queue
 @param h heap
 @param ix element to sift down
 */
void sift_down(heap *h, int ix) {
    int curr_node = ix;
    int children = get_child(curr_node);
    while (children < h->size) {
        int max_child = h->type == MAX_HEAP ? max_ix_proc(h->data, children, children+1, h->size) : min_ix_proc(h->data, children, children+1, h->size);
        if(compare_priority(h, curr_node, max_child) < 0) {
            swap_p(h->data, curr_node, max_child);
            curr_node = max_child;
            children = get_child(curr_node);
            continue;
        }
        break;
    }
}

/**
 Re-establishes the binary tree priority balance by sifting up an element. The element is hierarchically promoted until it's parent have a higher priority than it. This operation takes O(logN) time
 @param h heap
 @param ix element to sift up
 */
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

/**
 Pushes element in the heap in O(logN) time
 @param h heap
 @param p process to push
 */
void push(heap *h, process *p) {
    h->data[h->size] = p;
    h->size++;
    if(h->size > 1) {
        sift_up(h, h->size-1);
    }
}

/**
 Pops and returns the top most element from the heap, mantaining the binary tree balance
 @param h heap
 @return p highest priority process
 */
process* pop(heap *h) {
    process *popped = h->data[0];
    h->data[0] = h->data[h->size-1];
    h->size--;
    if(h->size > 1) {
        sift_down(h, 0);
    }
    return popped;
}

/**
 Returns the heap's top element without removing it
 @param h heap
 @return top most heap's process
 */
process* peek(heap *h) {
    return h->data[0];
}

/**
 Turns a given array into a heap
 @param arr the unheaped array
 @param type type of heap (min or max)
 @param heap_size heap's size (aka arr size)
 @return heapified array
 */
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
