#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <taskset.h>

#define BUFF_SIZE 256

// Below will error out the program
// if any invalid event occurs.
inline void errorAndDie(const char *errorMsg)
{
   perror(errorMsg);
   exit(EXIT_FAILURE);
}


FILE *openFile(const char *fileName)
{
   FILE *file;
   // Opens a file in read mode.
   if ((file = fopen(fileName, "rw")) == NULL)
   {
      // Errors out if the file did not open successfully.
      fputs("Error opening file\n", stderr);
      exit(EXIT_FAILURE);
   }
   return file;
}

inline task_sets *updateTaskSet(task_sets *currentTaskSet, char *lineFromFile)
{
   char *lineContents = strtok(lineFromFile, " ");

   char *name = lineContents[0];
   int wcet = atoi(lineContents[1]);
   int period = atoi(lineContents[2]);

   if (!isalpha(name))
      errorAndDie("Invalid task name");
   
   if(isdigit(wcet))
      errorAndDie("Invalid wcet");

   if (isdigit(period))
      errorAndDie("Invalid period");

   if (currentTaskSet == NULL)
      return create_task_set(name, wcet, period);
   else
      return add_task_set(currentTaskSet, name, wcet, period);
}

task_sets *extract_task_sets(char* fileName)
{
   FILE *file = openFile(fileName);
   char buff[BUFF_SIZE];
   task_sets *task_sets;

   while (fgets(buff, sizeof(buff), file) != EOF) {
      task_sets = updateTaskSet(task_sets, buff);
   }

   return task_sets;
}