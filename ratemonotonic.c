/*
 * Group Members:
 * Emmanuel A. Castillo, 004268444
 * Ashwin Paramashivan
 * Jesus Rivera
 * Edgar Cruz
 * Description:
 */
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <math.h>

#include "fileoperations.h"
#include "taskset.h"

int main(int argc, char *argv[])
{
  /* Validate argv (arguments). */
  if(argc != 4)
    {
      printf("Usage: ./task <nperiods> <taskset> <schedule>. \n", strerror(errno));
      return -1;
    }
  /* Set up variables to represent arguments. */
  char *taskSetFile; //= "taskset.txt"; // need to come from argv
  char *scheduleFile;// = "schedule.txt"; // need to come from argv
  int nperiods = atoi(argv[1]);
  taskSetFile = argv[2];
  scheduleFile = argv[3];

  /* Create a link list from task set file. */
  task_list *task_set_list = extract_task_sets(taskSetFile);

  /* Validate task set if it is scheduleable, if not error out. */

  float wcet1 = (get_task(task_set_list, 0)->wcet);
  float wcet2 = (get_task(task_set_list, 1)->wcet);
  float wcet3 = (get_task(task_set_list, 2)->wcet);
  float wcet4 = (get_task(task_set_list, 3)->wcet);
  float wcet5 = (get_task(task_set_list, 4)->wcet);
  float period1 = (get_task(task_set_list, 0)->period);
  float period2 = (get_task(task_set_list, 1)->period);
  float period3 = (get_task(task_set_list, 2)->period);
  float period4 = (get_task(task_set_list, 3)->period);
  float period5 = (get_task(task_set_list, 4)->period);


  float B = 5*(pow(2,0.2)-1);
  float U = (wcet1/period1) + (wcet2/period2) + (wcet3/period3) + (wcet4/period4) + (wcet5/period5);

  if(U >= B)
    {
      printf("The U:  %f, B: %f \n", U, B);
      printf("This is not schedulable.  \n", strerror(errno));
      return -1;
    }
  else
    {
      printf("The U:  %f, B: %f \n", U, B);
    }


  /* Determine hyper period(LCM) and put as first line of schedule.txt */

  float LCM = period1*period2*period3*period4*period5;



  /* Implement Rate Monotonic algorithm. */

  /* Display files of link list: example to show how to grab data from the link list. */
  printf("Task 1: %s, %d, %d\n", get_task(task_set_list, 0)->name, get_task(task_set_list, 0)->wcet, get_task(task_set_list, 0)->period);
  printf("Task 2: %s, %d, %d\n", get_task(task_set_list, 1)->name, get_task(task_set_list, 1)->wcet, get_task(task_set_list, 1)->period);
  printf("Task 3: %s, %d, %d\n", get_task(task_set_list, 2)->name, get_task(task_set_list, 2)->wcet, get_task(task_set_list, 2)->period);
  printf("Task 4: %s, %d, %d\n", get_task(task_set_list, 3)->name, get_task(task_set_list, 3)->wcet, get_task(task_set_list, 3)->period);
  printf("Task 5: %s, %d, %d\n", get_task(task_set_list, 4)->name, get_task(task_set_list, 4)->wcet, get_task(task_set_list, 4)->period);

  /* Write to schedule file. */
  write_to_file(scheduleFile, "This is a test\n");
  write_to_file(scheduleFile, get_task(task_set_list, 0)->name);
  write_to_file(scheduleFile, " ");
  write_to_file(scheduleFile, get_task(task_set_list, 1)->name);
  write_to_file(scheduleFile, " ");
  write_to_file(scheduleFile, get_task(task_set_list, 2)->name);
  write_to_file(scheduleFile, " ");
  write_to_file(scheduleFile, get_task(task_set_list, 3)->name);
  write_to_file(scheduleFile, " ");
  write_to_file(scheduleFile, get_task(task_set_list, 4)->name);
  write_to_file(scheduleFile, "\n");
  write_to_file(scheduleFile, "Mister.\n");

  return(EXIT_SUCCESS);
}
