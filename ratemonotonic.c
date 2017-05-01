/*
* Emmanuel A. Castillo, 004268444
* ecasti107@cougars.csusm.edu
* CS 351 Programming Languages
* 11/25/2016
* C1A4E1_main.c
* Win7
* Visual C++ 11.0
*
* Description:
* The below code uses the natoenglishtranslate.h
* to read in a words.txt file and display its
* corresponding translation. In addition,
* this file contains the implementations of the nato to english or vice versa
* translation. In addition, inline functions are used for the translation assistance.
* The following functions exist:
* 1) Shut program if error exist.
* 2) Open a file for reading.
* 3) Compare string insenstive to case.
* 4) Check what type of translation to occur based on the first 8 chars or lower of the parsed file.
* 5) Translate letters to NATO code.
* 6) Translate NATO code to letters.
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
