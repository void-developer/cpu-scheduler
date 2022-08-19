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

#define PROCESS_NUM 10
#define FALSE 0
#define TRUE 1
#define CPU_OP 0
#define IO_OP 1

int latest_printed_lines_num = 0;

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

void swap(int *a, int *b) {
    int temp = *a;
    *a = *b;
    *b = temp;
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
    latest_printed_lines_num = 7;
    printQueue(c_q);
    printw("\n### I/O QUEUE ###\n");
    printQueue(io_q);
    refresh();
}


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

int main() {
    if (!ensure_debugger_attached_woraround(700))
        return 1;
    
    initscr();
    fcfs();
    endwin();
    return 0;
}
