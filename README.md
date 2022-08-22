#  CPU Scheduler
A simple CPU scheduler simulator

## Start

To start the tool compile the program using the following command:

```
gcc main.c ./includes/mainhelper.c ./includes/process.c ./includes/queues.c ./includes/debugger.c ./includes/heaps.c ./includes/pmath.c ./includes/parrays.c -o main -lncurses -I./includes/
```

After compiling the program execute the generate executable with no additional parameters.
When running the program, it will ask the type of scheduling policy you would like to simulate, choose one. To try another one you must first terminate the current run and start a new one.

## Scheduling Policies

As of right now the scheduling policies available to simulate are: First Come First Serve (FCFS), Round Robin (RR), Virtual Round Robin (VRR) and Shortest Job First (SJF).
All policies have a CPU queue and an I/O queue. Once a process has terminated its CPU workload it will be moved to the I/O queue.
The processes are generated completely randomly and for each iteration the CPU burst time and the I/O time are regenerated in order to make process runtimes a little bit more dynamic

### First Come First Serve (FCFS)

The first come first serve policy has been implemented using a simple queue: a process enters the ready to run queue as the last to be executed of all the processes in the ready to run queue at that time

### Round Robin

Round Robin is also based on a simple queue system. The difference is that instead of waiting `cpu_burst_time` to be completed it interrupts the program when reaching the time quantum assigned, and then rescheduled in the ready to run queue (with the FCFS priority scheduling).

### Virtual Round Robin

With virtual round robin, a process that terminates its `cpu_burst_time` before consuming all of its `time_quantum`, will be assigned a `rr_credit` of the same amount of the remaining assigned `time_quantum`. A process that has been given credit, after terminating its I/O operations, will be scheduled on a separate queue: credit queue.
The scheduler now alternates getting processes from the ready to run queue and the credits ready to run queue

### Shortest Job First

The Shortes Job First scheduling policy has a binary tree heap as ready to run queue. The scheduler will run the tasks/processes with the shortest amount of CPU burst time. 
