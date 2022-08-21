//
//  process.h
//  cpu-scheduler
//
//  Created by Leonardo Angeli on 19/08/22.
//

#ifndef process_h
#define process_h

#include <stdio.h>
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
    int rr_credit;
    int time_quantum;
} process;
process null_process(void);
#endif /* process_h */
