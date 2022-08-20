//
//  mainhelper.c
//  cpu-scheduler
//
//  Created by Leonardo Angeli on 20/08/22.
//

#include "mainhelper.h"
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
#include "utility/parrays.h"

void reset_x_lines(int x) {
    printw("\33[2K");
    for (int i=0; i<x; i++) {
        printw("\033[A");
    }
}
  

process create_process(int pid, int priority, long arrival_time, int cpu_burst_time, int io_burst_time) {
    process process;
    process.pid = pid;
    process.priority = priority;
    process.arrival_time = arrival_time;
    process.cpu_burst_time = cpu_burst_time;
    process.io_burst_time = io_burst_time;
    process.waiting_time = 0;
    process.turnaround_time = 0;
    process.io_curr_burst_time = 0;
    process.cpu_curr_burst_time = 0;
    process.next_op = CPU_OP;
    process.time_quantum = 10;
    return process;
}

void randomize(int arr[], int n) {
    int i;
    srand((int)time(NULL));
    for (i = n - 1; i > 0; i--) {
        int j = rand() % (i + 1);
        swap(&arr[i], &arr[j]);
    }
}


void process_creation(queue *queue, process process_array[PROCESS_NUM]) {

    int pid_array[PROCESS_NUM];
    int i;
    for (i = 0; i < PROCESS_NUM; i++) {
        pid_array[i] = i + 1;
    }

    randomize(pid_array, PROCESS_NUM);

    srand((int)time(NULL));

    for (i = 0; i < PROCESS_NUM; i++) {
        time_t arrival_time = time(NULL);
        struct tm _tm = *localtime(&arrival_time);

        int priority = rand() % PROCESS_NUM;
        int cpu_burst_time = rand() % 20 + 1;
        int io_burst_time = rand() % 30;

        process_array[i] = create_process(pid_array[i], priority, _tm.tm_gmtoff, cpu_burst_time, io_burst_time);
        enqueue(queue, &process_array[i]);
    }
}

void arr_process_creation(process process_array[PROCESS_NUM]) {

    int pid_array[PROCESS_NUM];
    int i;
    for (i = 0; i < PROCESS_NUM; i++) {
        pid_array[i] = i + 1;
    }

    randomize(pid_array, PROCESS_NUM);

    srand((int)time(NULL));

    for (i = 0; i < PROCESS_NUM; i++) {
        time_t arrival_time = time(NULL);
        struct tm _tm = *localtime(&arrival_time);

        int priority = rand() % PROCESS_NUM;
        int cpu_burst_time = rand() % 20 + 1;
        int io_burst_time = rand() % 30;

        process_array[i] = create_process(pid_array[i], priority, _tm.tm_gmtoff, cpu_burst_time, io_burst_time);
        //enqueue(queue, &process_array[i]);
    }
}

void print_process(process *process) {
    printw("%2d      %4d     %8ld       %10d       %9d\n", process->pid, process->priority, process->arrival_time,
           process->cpu_burst_time, process->io_burst_time);
}

void print_active_process(process *process) {
    printw("%2d      %4d     %8ld       %10d       %9d      %13d      %13d", process->pid, process->priority,
           process->arrival_time, process->cpu_burst_time, process->io_burst_time, process->cpu_curr_burst_time,
           process->io_curr_burst_time);
}


void printQueue(queue *queue) {

    printw("pid    priority    arrival_time    cpu_burst_time    io_burst_time\n");
    printw("==================================================================\n");

    if (!is_empty(queue)) {
        node *tempNode = queue->head;
        while (tempNode != NULL) {
            print_process(tempNode->process);
            tempNode = tempNode->next;
        }
    }

    printw("===================================================================\n");
}


void print_status(queue *io_q, queue *c_q, process *c_p, process *io_p) {
    clear();
    printw("### CURRENTLY ACTIVE PROCESSES ###\n");
    printw("type   pid    priority    arrival_time    cpu_burst_time    io_burst_time    cpu_curr_burst_time    io_curr_burst_time\n");
    printw("================================================================================================================\n");
    printw("CPU -> ");
    if (c_p != NULL) {
        print_active_process(c_p);
    }
    printw("\n");
    printw("I/O -> ");
    if (io_p != NULL) {
        print_active_process(io_p);
    }
    printw("\n");
    printw("================================================================================================================\n\n");
    printw("### CPU PROCESS QUEUE ###\n");
    printQueue(c_q);
    printw("\n### I/O QUEUE ###\n");
    printQueue(io_q);
    refresh();
}

void print_heap(heap *h) {
    printw("pid    priority    arrival_time    cpu_burst_time    io_burst_time\n");
    printw("==================================================================\n");
    for (int i=0; i<h->size; i++) {
        print_process(&h->data[i]);
    }
    printw("===================================================================\n");
}

void print_status_heap(queue *io_q, heap *cpu_h, process *c_p, process *io_p) {
    clear();
    printw("### CURRENTLY ACTIVE PROCESSES ###\n");
    printw("type   pid    priority    arrival_time    cpu_burst_time    io_burst_time    cpu_curr_burst_time    io_curr_burst_time\n");
    printw("================================================================================================================\n");
    printw("CPU -> ");
    if (c_p != NULL) {
        print_active_process(c_p);
    }
    printw("\n");
    printw("I/O -> ");
    if (io_p != NULL) {
        print_active_process(io_p);
    }
    printw("\n");
    printw("================================================================================================================\n\n");
    printw("### CPU PROCESS QUEUE ###\n");
    print_heap(cpu_h);
    printw("\n### I/O QUEUE ###\n");
    printQueue(io_q);
    refresh();
}
