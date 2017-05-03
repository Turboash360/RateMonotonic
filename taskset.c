#include <stdio.h>
#include <stdlib.h>
#include <taskset.h>

task_sets *create_task_set(char* name, int wcet, int period) {
   task_sets* task_sets = (s_task_sets *) malloc(sizeof(task_sets));
   if (task_sets == NULL) {
      perror("Error initializing task set list.");
      exit(EXIT_FAILURE);
   }

   task_sets->name = name;
   task_sets->wcet = wcet;
   task_sets->period = period;

   return task_sets;
}

task_sets *add_task_set(task_sets* task_sets_head, char* name, int wcet, int period) 
{
   task_sets *new_task_set = create_task_set(name, wcet, period);

   if (new_task_set != NULL) {
      new_task_set->next = task_sets_head;
   }

   return new_task_set;
}
