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

task_run *TASK_RUN;

volatile int CURRENT_PERIOD = 0;
volatile int ENDING_PERIOD;

int isScheduleable()
{
    int size = TASK_SET_LIST->size;

    //Used for Utilization Bound test
    double worstCaseCpuUtil;  //B(n)
    double actualCpuUtil;  //summation of CPU uitilization

    int taskIndex;// used for receiving correct wcet or period based on the number

    for(taskIndex = 0; taskIndex < size; taskIndex++)
    {
        //CPU utilization: wcet/period
        actualCpuUtil += (double)(get_task(TASK_SET_LIST, taskIndex)->wcet) / (get_task(TASK_SET_LIST, taskIndex)->period);
    }

    printf("This is the size: %d\n", size);
    worstCaseCpuUtil = (size * (pow(2, (1/ (double)size)) -1));
    printf("Worst case: %f\n", worstCaseCpuUtil);
    printf("This is U: %f, This is B(n): %f\n", actualCpuUtil, worstCaseCpuUtil);

    if(actualCpuUtil > worstCaseCpuUtil)
        return 0;

    return 1;
}

/* Calculates the greatest common multiple. */
long calcGCM(long numOne, long numTwo)
{
    if(numOne == 0 || numTwo == 0) {
        return 0;
    }

    if(numOne == numTwo)
        return numOne;

    if(numOne > numTwo) {
        return calcGCM(numOne - numTwo, numTwo);
    }

    return calcGCM(numOne, numTwo - numOne);
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

    return (int)ceil(HYPER_PERIOD/gCM);
}

void printPeriodsScheduleInFile()
{
    int periodIndex;
    char buff[100];

    for(periodIndex = HYPER_PERIOD; periodIndex > 0; periodIndex--)
    {
        sprintf(buff, "%d", periodIndex - 1);
        write_to_file(SCHEDULE_FILE, buff);
        write_to_file(SCHEDULE_FILE, " ");
    }
    write_to_file(SCHEDULE_FILE, "\r\n");
}

void initiallyBlockAllTask()
{
    int taskIndex;
    for(taskIndex = 0; taskIndex < TASK_SET_LIST->size; taskIndex++){
        sem_wait(&TASK_SEMAPHORES[taskIndex]);
    }
}

void mutualExclusiveWriteToSchedFile(char *stringToWrite)
{
    pthread_mutex_lock(&SCHEDULE_FILE_MUTEX);
    write_to_file(SCHEDULE_FILE, stringToWrite);
    pthread_mutex_unlock(&SCHEDULE_FILE_MUTEX);
}

int compareWhichWillRun(int taskOne, int taskTwo)
{
    if(get_task(TASK_SET_LIST, taskOne)->wcet < get_task(TASK_SET_LIST, taskTwo)->wcet)
        return taskOne;

    return taskTwo;
}

int isRunnable(int taskIndex)
{
    int taskRunCounter = 0;

    /* All task run initially at period 0. */
    if(CURRENT_PERIOD == 0)
        return 1;

    /* Check if and task run exist to determine whether
     * task can run for the current period. */
    while(taskRunCounter < sizeof(TASK_RUN))
    {
        if(taskIndex == TASK_RUN->taskIndex) {
            if(TASK_RUN->periodRan < CURRENT_PERIOD)
                return 1;
        }
        taskRunCounter++;
    }

    /* If task period is greater or equal to the current period, it is runnable. */
    if(get_task(TASK_SET_LIST, taskIndex)->period * (HYPER_PERIOD / CURRENT_PERIOD)>= CURRENT_PERIOD) {
        return 1;
    }

    return 0;
}

void determineWhatRuns()
{
    int taskIndex;

    /**** The below commented code we are stuck with a dead lock. ****/
//    int taskIndexThatCanRun = TASK_SET_LIST->size + 1;
//
//    /* Determine if any task can be ran. */
//    for(taskIndex = 0; taskIndex < TASK_SET_LIST->size; taskIndex++){
//        // Find what can run
//        if(isRunnable(taskIndex)) {
//            if(taskIndexThatCanRun == TASK_SET_LIST->size + 1) {
//                taskIndexThatCanRun = taskIndex;
//            } else {
//                taskIndexThatCanRun = compareWhichWillRun(taskIndexThatCanRun, taskIndex);
//            }
//        }
//    }
//
//    /* If no run, print a space. */
//    if(taskIndexThatCanRun == TASK_SET_LIST->size + 1) {
//        mutualExclusiveWriteToSchedFile(" ");
//    } else {
//    /* If run, wait all task except one that will run. */
//        for(taskIndex = 0; taskIndex < TASK_SET_LIST->size; taskIndex++){
//            if(taskIndex == taskIndexThatCanRun){
//                sem_wait(&(TASK_SEMAPHORES[taskIndex]));
//            }
//        }
//    }

     for(taskIndex = 0; taskIndex < TASK_SET_LIST->size; taskIndex++){
        if(get_task(TASK_SET_LIST, taskIndex)->period == CURRENT_PERIOD) {
            sem_post(&(TASK_SEMAPHORES[taskIndex]));
        }
    }
}

void runSchedule()
{
    for(CURRENT_PERIOD = 0; CURRENT_PERIOD < ENDING_PERIOD; CURRENT_PERIOD++)
    {
        determineWhatRuns();

        if(CURRENT_PERIOD != 0)
        {
            /* Go to next row after 1 hyper period. */
            if(HYPER_PERIOD % CURRENT_PERIOD == 0)
                mutualExclusiveWriteToSchedFile("\n");
        }
    }
}

/* When task's semaphore is available, it prints to the schedule file. */
void *runTask(void *arg)
{
    int taskIndex = (int)arg;

    while(CURRENT_PERIOD < ENDING_PERIOD)
    {
        sem_wait(&(TASK_SEMAPHORES[taskIndex]));
        mutualExclusiveWriteToSchedFile(get_task(TASK_SET_LIST, taskIndex)->name);
        mutualExclusiveWriteToSchedFile(" ");
        sem_post(&(TASK_SEMAPHORES[taskIndex]));
    }
}

void runRateMonotonicAlgorithm()
{
    TASK_SEMAPHORES = malloc(sizeof(sem_t) * TASK_SET_LIST->size);
    pthread_t taskThreads[TASK_SET_LIST->size];
    int index;

    /* Set up semaphore for each tasks*/
    for(index = 0; index < TASK_SET_LIST->size; index++)
    {
        if(sem_init(&TASK_SEMAPHORES[index], 0, 1) != 0){
            perror("Error creating thread.\n");
            exit(EXIT_FAILURE);
        }
    }

    /* Block all semaphores until scheduling algorithm takes place. */
    initiallyBlockAllTask();

    /* Set up threads for each tasks */
    if(pthread_mutex_init(&SCHEDULE_FILE_MUTEX, NULL) != 0)
    {
        perror("Error creating mutex.\n");
        exit(EXIT_FAILURE);
    }

    /* Used to keep track of task that ran. */
    TASK_RUN = malloc(sizeof(task_run) * TASK_SET_LIST->size);

    /* Set up threads for tasks */
    for(index = 0; index < TASK_SET_LIST->size; index++)
    {
        if(pthread_create(&(taskThreads[index]), NULL, &runTask, (void *)index) != 0){
            perror("Error creating thread.\n");
            exit(EXIT_FAILURE);
        }
    }

    /* Run schedule to manage threads. */
    runSchedule();

    /* Clean up threads. */
    for(index = 0; index < TASK_SET_LIST->size; index++){
        pthread_join(taskThreads[index], NULL);
    }

    /* Clean up mutex */
    pthread_mutex_destroy(&SCHEDULE_FILE_MUTEX);
}

int main(int argc, char *argv[])
{
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
    HYPER_PERIOD = NUM_HYPER_PERIOD;//calcHyperPeriod();
    printPeriodsScheduleInFile();

    /* Total periods for the task based on hyper period and the user
        input number of hyper periods (nperiods)                    */
    ENDING_PERIOD = HYPER_PERIOD * NUM_HYPER_PERIOD;

   /* Implement Rate Monotonic algorithm. */
   runRateMonotonicAlgorithm();

   return(EXIT_SUCCESS);
}
