-------------------------------------------------------
INTRODUCTION
-------------------------------------------------------
Class: CS 433 - Operating System
Rate Monotonic
Executable file: rmsched.c
Test file: taskset.txt, schedule.txt

-------------------------------------------------------
DESCRIPTION
-------------------------------------------------------
Develop a program that simulates a rate monotonic
scheduling algorithm among a number of threads
defined by a taskset.txt file.

-------------------------------------------------------
RUNNING EXECUTABLE FILE ONLY
-------------------------------------------------------
Refer to Step 4 and/or Step 5 
in the "COMPILING & RUNNING" section below.

-------------------------------------------------------
COMPILING & RUNNING
-------------------------------------------------------
Step 1: Make sure the following file exist in your directory:
1) c & h files
 a) fileoperations.c
 b) fileoperations.h
 c) rmsched.c (Main program)
 d) taskset.c
 e) taskset.h
2) Test files
 a) schedule.txt
 b) taskset.txt

Step 2: Compile code
Example-> gcc *.c -o rmsched -lpthread -lm

Step 3: Run code
./rmsched 9 taskset.txt schedule.txt
