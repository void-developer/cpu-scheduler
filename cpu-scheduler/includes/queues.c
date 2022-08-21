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

void init(queue *q) {
    q->size = 0;
    q->head = NULL;
    q->head = NULL;
}

int is_empty(queue *q) {
    return (q->head == NULL);
}

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

process *dequeue(queue *q) {
    node *tmp = q->head;
    process *p = tmp->process;
    q->head = tmp->next;
    q->size--;
    free(tmp);
    return p;
}
