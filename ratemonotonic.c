/*
 * Group Members:
 * Emmanuel A. Castillo, 004268444
 * testing
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
   // validate arguments

   char *fileName = "taskset.txt"; // need to come from argv

   // read file and create TASK_SET struct array
   task_sets *task_sets = extract_task_sets(fileName);


   // validate file contents

   // validate that task set is schedulable

   // set up rate monotonic schedule

   // run threads accordingly


   return(EXIT_SUCCESS);
}
