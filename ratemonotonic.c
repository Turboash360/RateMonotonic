/*
* Group Members:
* Emmanuel A. Castillo, 004268444
* 
* Description:
*/
#include <ctype.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <fileoperations.h>
#include <taskset.h>

int main(int argc, char *argv[])
{
   /* Validate argv (arguments). */

   /* Set up variables to represent arguments. */
   char *taskSetFile = "taskset.txt"; // need to come from argv
   char *scheduleFile = "schedule.txt"; // need to come from argv

   /* Create a link list from task set file. */
   task_list *task_set_list = extract_task_sets(taskSetFile);

   /* Validate task set if it is scheduleable, if not error out. */

   /* Implement Rate Monotonic algorithm. */

   /* Display files of link list: example to show how to grab data from the link list. */
   printf("Task 1: %s\n", get_task(task_set_list, 0)->name);
   printf("Task 2: %s\n", get_task(task_set_list, 1)->name);
   printf("Task 3: %s\n", get_task(task_set_list, 2)->name);
   printf("Task 4: %s\n", get_task(task_set_list, 3)->name);

   /* Write to schedule file. */
   write_to_file(scheduleFile, "This is a test\n");
   write_to_file(scheduleFile, get_task(task_set_list, 0)->name);
   write_to_file(scheduleFile, " ");
   write_to_file(scheduleFile, get_task(task_set_list, 1)->name);
   write_to_file(scheduleFile, " ");
   write_to_file(scheduleFile, get_task(task_set_list, 2)->name);
   write_to_file(scheduleFile, " ");
   write_to_file(scheduleFile, get_task(task_set_list, 3)->name);
   write_to_file(scheduleFile, "\n");
   write_to_file(scheduleFile, "Mister.\n");

   return(EXIT_SUCCESS);
}