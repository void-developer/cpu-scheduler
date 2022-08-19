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

#define PROCESS_NUM 10
#define FALSE 0
#define TRUE 1
#define CPU_OP 0
#define IO_OP 1

int latest_printed_lines_num = 0;

typedef struct Process {
    int pid;
    int cpu_burst_time;
    int io_burst_time;
    long arrival_time;
    int priority;
    int io_curr_burst_time;
    int cpu_curr_burst_time;
    int next_op; //0 for CPU and 1 for I/O
    int waiting_time;
    int turnaround_time;
    int time_quantum;
} process;

typedef struct Node {
    process *process;
    struct Node *next;
} node;

typedef struct Queue {
    int size;
    node *head;
    node *tail;
} queue;

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

//############

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

// A function to generate a random permutation of arr[]
void randomize(int arr[], int n) {
    int i;
    // Use a different seed value so that we don't get same
    // result each time we run this program
    srand((int)time(NULL));

    // Start from the last element and swap one by one. We don't
    // need to run for the first element that's why i > 0
    for (i = n - 1; i > 0; i--) {
        // Pick a random index from 0 to i
        int j = rand() % (i + 1);

        // Swap arr[i] with the element at random index
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
//        time_t arrival_time = time;
        int cpu_burst_time = rand() % 20 + 1;
        int io_burst_time = rand() % 30;

        process_array[i] = create_process(pid_array[i], priority, _tm.tm_gmtoff, cpu_burst_time, io_burst_time);
        enqueue(queue, &process_array[i]);
//        sleep(2);
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

void *handle_io_operations(queue *io_q, queue *c_q, void *par) {
    while (!is_empty(c_q) && !is_empty(io_q)) {
        process *p = dequeue(io_q);
        sleep(p->io_burst_time);
        p->next_op = 0;
        enqueue(c_q, p);
    }
    pthread_exit(par);
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
    initscr();
//    raw();
//    printw("Hello world!");
    fcfs();
//    getch();
    endwin();
    return 0;
}
