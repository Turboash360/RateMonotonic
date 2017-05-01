/*
* Emmanuel A. Castillo, 004268444
*
sdfadfdssdfafsd* Description:
*/
#include <ctype.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


typedef struct {
   char* taskName;
   int wcet;
   int period;
} TASK_SET;


// Below will error out the program
// if any invalid event occurs.
inline void errorAndDie(const char *errorMsg)
{
   perror(errorMsg);
   exit(EXIT_FAILURE);
}

inline FILE *openFile(const char *fileName)
{
   FILE *file;
   // Opens a file in read mode.
   if ((file = fopen(fileName, "r")) == NULL)
   {
      // Errors out if the file did not open successfully.
      fputs("Error opening file\n", stderr);
      exit(EXIT_FAILURE);
   }
   return file;
}


int main(int argc, char *argv[])
{
   // validate arguments

   // read file and create TASK_SET struct array

   // validate file contents

   // validate that task set is schedulable

   // set up rate monotonic schedule

   // run threads accordingly


   return(EXIT_SUCCESS);
}
