//
//  main.c
//  cpu-scheduler
//
//  Created by Leonardo Angeli on 19/08/22.
//

//
// Created by Leonardo Angeli on 18/08/22.
//
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <pthread.h>
#include <string.h>
#include <curses.h>
#include "process.h"
#include "queues.h"
#include "debugger.h"
#include "heaps.h"
#include "mainhelper.h"

#define FCFS 0
#define SJF 1
#define RR 2
#define VRR 3

int latest_printed_lines_num = 0;

#define CREDIT_QUEUE 1
#define STD_QUEUE 0

void fcfs(void) {
    process cpu_array[PROCESS_NUM];
    queue *c_q = (queue *) malloc(sizeof(queue));
    queue *io_q = (queue *) malloc(sizeof(queue));
    process *io_p = NULL;
    process *c_p = NULL;
    init(c_q);
    init(io_q);
    process_creation(c_q, cpu_array);
    print_status(io_q, c_q, c_p, io_p);
    int time = 0;
    while (!is_empty(c_q) || !is_empty(io_q)) {
        time++;
        sleep(1);
        if (io_p == NULL && !is_empty(io_q)) {
            io_p = dequeue(io_q);
        }
        if (c_p == NULL && !is_empty(c_q)) {
            c_p = dequeue(c_q);
        }

        if (c_p != NULL && c_p->cpu_curr_burst_time < c_p->cpu_burst_time) {
            c_p->cpu_curr_burst_time++;
        } else if (c_p != NULL) {
            c_p->cpu_curr_burst_time = 0;
            c_p->next_op = IO_OP;
            enqueue(io_q, c_p);
            c_p = NULL;
        }
        if (io_p != NULL && io_p->io_curr_burst_time < io_p->io_burst_time) {
            io_p->io_curr_burst_time++;
        } else if (io_p != NULL) {
            io_p->io_curr_burst_time = 0;
            io_p->next_op = CPU_OP;
            enqueue(c_q, io_p);
            io_p = NULL;
        }
        print_status(io_q, c_q, c_p, io_p);
    }
}

void sjf(void) {
    process cpu_array[PROCESS_NUM];
    queue *io_q = (queue *) malloc(sizeof(queue));
    process *io_p = NULL;
    process *c_p = NULL;
    init(io_q);
    arr_process_creation(cpu_array);

    //initialize process priority with cpu burst time length
    for (int i=0; i<PROCESS_NUM; i++) {
        cpu_array[i].priority = cpu_array[i].cpu_burst_time;
    }
    heap *cpu_h = heapify(cpu_array, MIN_HEAP, sizeof(cpu_array)/sizeof(cpu_array[0]));
    
    print_status_heap(io_q, cpu_h, c_p, io_p);
    int time = 0;
    while (!is_heap_empty(cpu_h) || !is_empty(io_q)) {
        time++;
        sleep(1);
        if (io_p == NULL && !is_empty(io_q)) {
            io_p = dequeue(io_q);
        }
        if (c_p == NULL && !is_heap_empty(cpu_h)) {
            c_p = pop(cpu_h);
        }

        if (c_p != NULL && c_p->cpu_curr_burst_time < c_p->cpu_burst_time) {
            c_p->cpu_curr_burst_time++;
        } else if (c_p != NULL) {
            c_p->cpu_curr_burst_time = 0;
            c_p->next_op = IO_OP;
            enqueue(io_q, c_p);
            c_p = NULL;
        }
        if (io_p != NULL && io_p->io_curr_burst_time < io_p->io_burst_time) {
            io_p->io_curr_burst_time++;
        } else if (io_p != NULL) {
            io_p->io_curr_burst_time = 0;
            io_p->next_op = CPU_OP;
            push(cpu_h, io_p);
            io_p = NULL;
        }
        print_status_heap(io_q, cpu_h, c_p, io_p);
    }
}

void rr(void) {
    int tq = 5;
    process cpu_array[PROCESS_NUM];
    queue *c_q = (queue *) malloc(sizeof(queue));
    queue *io_q = (queue *) malloc(sizeof(queue));
    process *io_p = NULL;
    process *c_p = NULL;
    init(c_q);
    init(io_q);
    process_creation(c_q, cpu_array);
    print_status(io_q, c_q, c_p, io_p);
    int time = 0;
    while (!is_empty(c_q) || !is_empty(io_q)) {
        time++;
        sleep(1);
        if (io_p == NULL && !is_empty(io_q)) {
            io_p = dequeue(io_q);
        }
        if (c_p == NULL && !is_empty(c_q)) {
            c_p = dequeue(c_q);
        }

        if (c_p != NULL && c_p->cpu_curr_burst_time < c_p->cpu_burst_time && c_p->time_quantum > 0) {
            c_p->cpu_curr_burst_time++;
            c_p->time_quantum--;
        } else if(c_p != NULL && c_p->time_quantum <= 0) {
            c_p->time_quantum = tq;
            enqueue(c_q, c_p);
            c_p = NULL;
        } else if (c_p != NULL) {
            c_p->cpu_curr_burst_time = 0;
            c_p->time_quantum = tq;
            c_p->io_burst_time = rand() % 30;
            c_p->next_op = IO_OP;
            enqueue(io_q, c_p);
            c_p = NULL;
        }
        
        if (io_p != NULL && io_p->io_curr_burst_time < io_p->io_burst_time) {
            io_p->io_curr_burst_time++;
        } else if (io_p != NULL) {
            io_p->io_curr_burst_time = 0;
            io_p->cpu_burst_time = rand() % 20 + 1;
            io_p->next_op = CPU_OP;
            enqueue(c_q, io_p);
            io_p = NULL;
        }
        print_status(io_q, c_q, c_p, io_p);
    }
}


void vrr(void) {
    int tq = 10;
    process cpu_array[PROCESS_NUM];
    queue *c_q = (queue *) malloc(sizeof(queue));
    queue *cc_q = (queue *) malloc(sizeof(queue));
    queue *io_q = (queue *) malloc(sizeof(queue));
    process *io_p = NULL;
    process *c_p = NULL;
    init(c_q);
    init(io_q);
    process_creation(c_q, cpu_array);
    print_status_vrr(io_q, c_q, cc_q, c_p, io_p);
    
    int prev_chosen_q = CREDIT_QUEUE;
    int time = 0;
    while (!is_empty(c_q) || !is_empty(io_q)) {
        time++;
        sleep(1);
        if (io_p == NULL && !is_empty(io_q)) {
            io_p = dequeue(io_q);
        }
        if (c_p == NULL && (!is_empty(c_q) || !is_empty(cc_q))) {
            if((prev_chosen_q == CREDIT_QUEUE && !is_empty(c_q)) || is_empty(cc_q)) {
                prev_chosen_q = STD_QUEUE;
                c_p = dequeue(c_q);
            } if((prev_chosen_q == STD_QUEUE && !is_empty(cc_q)) || is_empty(c_q)) {
                prev_chosen_q = CREDIT_QUEUE;
                c_p = dequeue(cc_q);
            }
        }

        if (c_p != NULL && c_p->cpu_curr_burst_time < c_p->cpu_burst_time && c_p->time_quantum > 0) {
            c_p->cpu_curr_burst_time++;
            c_p->time_quantum--;
        } else if(c_p != NULL && c_p->time_quantum <= 0) {
            if(c_p->rr_credit > 0) {
                c_p->rr_credit--;
                c_p->time_quantum--;
                c_p->cpu_curr_burst_time++;
            } else {
                c_p->time_quantum = tq;
                enqueue(c_q, c_p);
                c_p = NULL;
            }
        } else if (c_p != NULL) {
            c_p->cpu_curr_burst_time = 0;
            c_p->rr_credit = c_p->time_quantum;
            c_p->time_quantum = tq;
            c_p->io_burst_time = rand() % 30;
            c_p->next_op = IO_OP;
            enqueue(io_q, c_p);
            c_p = NULL;
        }
        
        if (io_p != NULL && io_p->io_curr_burst_time < io_p->io_burst_time) {
            io_p->io_curr_burst_time++;
        } else if (io_p != NULL) {
            io_p->io_curr_burst_time = 0;
            io_p->cpu_burst_time = rand() % 20 + 1;
            io_p->next_op = CPU_OP;
            if(io_p->rr_credit > 0)
                enqueue(cc_q, io_p);
            else
                enqueue(c_q, io_p);
            io_p = NULL;
        }
        print_status_vrr(io_q, c_q, cc_q, c_p, io_p);
    }
}



int main() {
    int choice = FCFS;
    printf("Choose the CPU scheduling policy to simulate: ");
    printf("[0: FCFS, 1: SJF, 2: RR, 3: VRR]: ");
    scanf("%d", &choice);
    while(choice < 0 || choice > 3) {
        printf("[0: FCFS, 1: SJF, 2: RR, 3: VRR]: ");
        scanf("%d", &choice);
    }
    initscr();
    switch (choice) {
        case FCFS:
            fcfs();
            break;
        case SJF:
            sjf();
            break;
        case RR:
            rr();
            break;
        case VRR:
            vrr();
            break;
        default:
            break;
    }
    endwin();
}
