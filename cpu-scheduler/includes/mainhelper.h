//
//  mainhelper.h
//  cpu-scheduler
//
//  Created by Leonardo Angeli on 20/08/22.
//

#ifndef mainhelper_h
#define mainhelper_h

#include <stdio.h>
#include "process.h"
#include "heaps.h"
#include "queues.h"
#include <curses.h>
#define PROCESS_NUM 10
#define FALSE 0
#define TRUE 1
#define CPU_OP 0
#define IO_OP 1
process create_process(int pid, int priority, long arrival_time, int cpu_burst_time, int io_burst_time);
void process_creation(queue *queue, process process_array[PROCESS_NUM]);
void print_process(process *process);
void print_active_process(process *process);
void printQueue(queue *queue);
void print_status(queue *io_q, queue *c_q, process *c_p, process *io_p);
void print_status_heap(queue *io_q, heap *c_q, process *c_p, process *io_p);
void arr_process_creation(process process_array[PROCESS_NUM]);
void print_status_vrr(queue *io_q, queue *c_q, queue *cc_q, process *c_p, process *io_p);
#endif /* mainhelper_h */
