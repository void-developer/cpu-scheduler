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
#include "datastructs/queues.h"
#include "utility/debugger.h"
#include "datastructs/heaps.h"
#include "mainhelper.h"

int latest_printed_lines_num = 0;

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
    
    heap *cpu_h = heapify(cpu_array, MIN_HEAP);
    
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

int main() {
    if (!ensure_debugger_attached_woraround(700))
        return 1;
    
    initscr();
//    fcfs();
    sjf();
    endwin();
    return 0;
}
