//
//  queues.h
//  cpu-scheduler
//
//  Created by Leonardo Angeli on 19/08/22.
//

#ifndef queues_h
#define queues_h

#include <stdio.h>
#include "process.h"
typedef struct Node {
    process *process;
    struct Node *next;
} node;

typedef struct Queue {
    int size;
    node *head;
    node *tail;
} queue;

void init(queue *q);
int is_empty(queue *q);
void enqueue(queue *q, process *p);
process *dequeue(queue *q);
#endif /* queues_h */
