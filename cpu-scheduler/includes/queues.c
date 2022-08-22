//
//  queues.c
//  cpu-scheduler
//
//  Created by Leonardo Angeli on 19/08/22.
//

#include "queues.h"
#include "process.h"
#include <stdio.h>
#include <stdlib.h>

/**
 Initializes the queue with the default parameters.
 Initial size is 0, head and tail are both NULL
 @param q queue to initialize
*/
void init(queue *q) {
    q->size = 0;
    q->head = NULL;
    q->tail = NULL;
}

/**
 Checks whether the given queue is empty
 @param q queue
 */
int is_empty(queue *q) {
    return (q->head == NULL);
}

/**
 Adds a process to the tail of the queue
 @param q queue
 @param p the process to add to the queue
*/
void enqueue(queue *q, process *p) {
    node *new_node;
    new_node = (node *) malloc(sizeof(node));
    new_node->process = p;
    new_node->next = NULL;
    if (is_empty(q)) {
        q->head = new_node;
        q->tail = new_node;
        return;
    }
    q->tail->next = new_node;
    q->tail = new_node;
    q->size++;
    return;
}

/**
 Removes the top element of the queue and returns it from the function
 @param q queue
*/
process *dequeue(queue *q) {
    node *tmp = q->head;
    process *p = tmp->process;
    q->head = tmp->next;
    q->size--;
    free(tmp);
    return p;
}
