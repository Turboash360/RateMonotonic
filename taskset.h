#ifndef TASKSET_H
#define TASKSET_H

typedef struct s_task_sets {
   char* name;
   int wcet;
   int period;
   struct s_task_sets* next;
} task_sets;

task_sets *create_task_set(char* name, int wcet, int period);

task_sets *add_task_set(task_sets* task_sets_head, char* name, int wcet, int period);

#endif