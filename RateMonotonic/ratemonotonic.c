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

#include <pthread.h>
#include <semaphore.h>

#include "fileoperations.h"
#include "taskset.h"

task_list *TASK_SET_LIST;
pthread_mutex_t SCHEDULE_FILE_MUTEX;
sem_t *TASK_SEMAPHORES;
char *SCHEDULE_FILE;
int NUM_HYPER_PERIOD;
int HYPER_PERIOD;

int isScheduleable()
{
    int size = TASK_SET_LIST->size;

    double worstCaseCpuUtil;
    double actualCpuUtil;

    int taskIndex;

    for(taskIndex = 0; taskIndex < size; taskIndex++)
    {
        actualCpuUtil += (get_task(TASK_SET_LIST, taskIndex)->wcet / get_task(TASK_SET_LIST, taskIndex)->period);
    }

    worstCaseCpuUtil = size * (pow(2, (1/ size)) -1);

    if(actualCpuUtil > worstCaseCpuUtil)
        return 0;

    return 1;
}

/* Calculates the greatest common multiple. */
long calcGCM(long numOne, long numTwo)
{
    if(numTwo == 0) {
        return numTwo;
    }

    return calcGCM(numTwo, numOne % numTwo);
}

/* Hyper period is based on the least common multiple of all the task sets. */
int calcHyperPeriod()
{
    long hyperPeriod; // The least common multiple of all the task sets.
    int taskSetSize = TASK_SET_LIST->size;
    int taskIndex;
    long allPeriodsMultiplied = 1;
    long gCM;

    for(taskIndex = 0; taskIndex < taskSetSize; taskIndex++)
    {
        allPeriodsMultiplied *= get_task(TASK_SET_LIST, taskIndex)->period;
    }

    for(taskIndex = 0; taskIndex < taskSetSize; taskIndex += 2)
    {
        if(taskIndex + 2 > taskSetSize)
            break;

        gCM = calcGCM(get_task(TASK_SET_LIST, taskIndex)->period, get_task(TASK_SET_LIST, taskIndex + 1)->period);
    }

    return (int)ceil(hyperPeriod/gCM);
}

void printPeriodsScheduleInFile()
{
    int periodIndex;
    char buff[100];

    for(periodIndex = 0; periodIndex < HYPER_PERIOD; periodIndex++)
    {
       // itoa(get_task(TASK_SET_LIST, periodIndex)->name, buff, sizeof(get_task(TASK_SET_LIST, periodIndex)->name));
        write_to_file(SCHEDULE_FILE, buff);
    }

}

void *runTask(void *arg)
{
    pthread_mutex_lock(&SCHEDULE_FILE_MUTEX);
  //  sem_wait();
    write_to_file(SCHEDULE_FILE, "");
    //sem_post();
    pthread_mutex_unlock(&SCHEDULE_FILE_MUTEX);

}

void runRateMonotonicAlgorithm()
{
    pthread_t taskThreads[TASK_SET_LIST->size];
  //  sem_init();
    int threadIndex;

    if(pthread_mutex_init(&SCHEDULE_FILE_MUTEX, NULL) != 0) {
        perror("Error creating mutex.\n");
            exit(EXIT_FAILURE);
    }

    for(threadIndex = 0; threadIndex < HYPER_PERIOD; threadIndex++)
    {
        if(pthread_create(&(taskThreads[threadIndex]), NULL, &runTask, NULL) != 0){
            perror("Error creating thread.\n");
            exit(EXIT_FAILURE);
        }
    }

    for(threadIndex = 0; threadIndex < HYPER_PERIOD; threadIndex++)
    {
        pthread_join(taskThreads[threadIndex], NULL);
    }

    pthread_mutex_destroy(&SCHEDULE_FILE_MUTEX);
}

int main(int argc, char *argv[])
{
   /* Validate argv (arguments). */  // TODO: Validation.

   /* Set up variables to represent arguments. */
   char *taskSetFile = argv[2];
   SCHEDULE_FILE = argv[3];
   NUM_HYPER_PERIOD = atoi(argv[1]); // TODO: Verify it is a digit.

   /* Create a link list from task set file. */
   TASK_SET_LIST = extract_task_sets(taskSetFile);

   /* Validate task set if it is scheduleable, if not error out. */
    if(!isScheduleable()) {
        perror("Tasks are not schedulable.\n");
        exit(EXIT_FAILURE);
    }

   /* Determine hyper period and set periods as first line of schedule.txt */
    HYPER_PERIOD = calcHyperPeriod();
    printPeriodsScheduleInFile();

   /* Implement Rate Monotonic algorithm. */
   runRateMonotonicAlgorithm();


   return(EXIT_SUCCESS);
}
