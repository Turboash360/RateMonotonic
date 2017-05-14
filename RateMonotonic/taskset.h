#ifndef TASKSET_H
#define TASKSET_H

typedef struct task {
   char* name; // name of task
   int wcet;   // worst case execution time
   int period; // when the task gets run
} task;

typedef struct task_run {
    int taskIndex;
    int periodRan;
} task_run;

typedef struct task_link {
   task *task;
   struct task_link *next;
} task_link;

typedef struct task_list {
   task_link* tasks;
   int size;
} task_list;

task_list* initialize_task_set_list();

void add_task_set_list(task_link *link, char* name, int wcet, int period);

task *get_task(task_list *link, int index);

#endif
