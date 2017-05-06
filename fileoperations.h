#ifndef FILEOPERATIONS_H
#define FILEOPERATIONS_H

#include "taskset.h"

task_list* extract_task_sets(char* fileName);

void write_to_file(char* fileName, char* stringToWrite);

#endif