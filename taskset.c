#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "taskset.h"

task_list* initialize_task_set_list()
{
   task_list *task_list = malloc(sizeof(task_list));
   task_list->tasks = NULL;
   task_list->size = 0;

   return task_list;
}

void add_task_set_list(task_link *link, char* name, int wcet, int period)
{
   task *new_task = malloc(sizeof(task *));
   new_task->name = malloc(sizeof(name));
   strcpy(new_task->name, name);
   new_task->wcet = wcet;
   new_task->period = period;

   if (link == NULL) {
      perror("Error initializing task set link.");
      exit(EXIT_FAILURE);
   }

   link->task = new_task;
   link->next = NULL;     
}

task *get_task(task_list *list, int index)
{
   task_link *link = list->tasks;
   if (index > sizeof(link) || index < 0)
   {
      perror("Index cannot be bigger than number of tasks or less than 0.");
      exit(EXIT_FAILURE);
   }

   task *task = link->task;

   while (index > 0) {
      link = link->next;
      task = link->task;
      index--;
   }
   return task;
}
