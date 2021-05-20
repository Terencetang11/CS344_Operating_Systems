/* 
*  Name : Terence Tang
*  Course : CS344 - Operating Systems
*  Date : May 16, 2021
*  Assignment #4: Multithreaded Producers and Consumers Pipeline
*  Description:  Simple multithreaded implementation for reformating text input from stdin to stdout.  Features
*                multithreaded implementation which highlights how to handle common issues with producer and 
*                consumer pipelines, e.g. mutual exclusivity of access to shared resources (mutex) and use of
*                conditional variables to help with thread signaling.
*/

// compiled using gcc option --std=c99

#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <pthread.h>
#include <unistd.h>
#include "multithreaded.h"

// Declare Global Resources 
bool stop_input = false;        // bool for if STOP has been correctly entered into stdin

// All thread buffers are equal to the max number of itmes to be produced.  Hence, we can model the buffer as being unbounded.
// Thread 1 variables - Buffers, producer and consumer indexes, mutex and cond vars.
char **buffer_1;                                        // Declare buffer to store lines of str input
int count_1 = 0;                                        // Number of items in the buffer 1
int prod_idx_1 = 0;                                     // Index where the input thread will put the next item
int con_idx_1 = 0;                                      // Index where the line separator thread will pick up the next item
pthread_mutex_t mutex_1 = PTHREAD_MUTEX_INITIALIZER;    // Initialize the mutex for buffer 1
pthread_cond_t full_1 = PTHREAD_COND_INITIALIZER;       // Initialize the condition variable for buffer 1

// Thread 2 variables - Buffers, producer and consumer indexes, mutex and cond vars.
char **buffer_2;                                        // Declare buffer to store lines of line seperator cleaned strs
int count_2 = 0;                                        // Number of items in the buffer 2
int prod_idx_2 = 0;                                     // Index where the line separator thread will put the next item
int con_idx_2 = 0;                                      // Index where the plus sign thread will pick up the next item
pthread_mutex_t mutex_2 = PTHREAD_MUTEX_INITIALIZER;    // Initialize the mutex for buffer 2
pthread_cond_t full_2 = PTHREAD_COND_INITIALIZER;       // Initialize the condition variable for buffer 2

// Thread 3 variables - Buffers, producer and consumer indexes, mutex and cond vars.
char **buffer_3;                                        // Declare buffer to store lines of plus sign cleaned strs
int count_3 = 0;                                        // Number of items in the buffer 3
int prod_idx_3 = 0;                                     // Index where the plus sign thread will put the next item
int con_idx_3 = 0;                                      // Index where the output thread will pick up the next item
pthread_mutex_t mutex_3 = PTHREAD_MUTEX_INITIALIZER;    // Initialize the mutex for buffer 3
pthread_cond_t full_3 = PTHREAD_COND_INITIALIZER;       // Initialize the condition variable for buffer 3


/* 
* Frees all dynamically allocated memory elements as part of the shared program buffers across
* the input, line separator, plus sign, and output threads
*/
int free_mem(char **buffer_1, char **buffer_2, char **buffer_3)
{
    // resets command line input datastructure
    int index = 0;
    char *buffer1_line = buffer_1[index];
    while (buffer1_line != NULL)
    {
        free(buffer1_line);
        index++;
        buffer1_line = buffer_1[index];
    }
    free(buffer_1);

    index = 0;
    char *buffer2_line = buffer_2[index];
    while (buffer2_line != NULL)
    {
        free(buffer2_line);
        index++;
        buffer2_line = buffer_2[index];
    }
    free(buffer_2);

    index = 0;
    char *buffer3_line = buffer_3[index];
    while (buffer3_line != NULL)
    {
        free(buffer3_line);
        index++;
        buffer3_line = buffer_3[index];
    }
    free(buffer_3);
    return 1;
}


int main()
{
    // initialize shared program resources with declared max sizes
    buffer_1 = malloc((MAX_PROGRAM_SIZE) * sizeof(char*));
    buffer_2 = malloc((MAX_PROGRAM_SIZE) * sizeof(char*));
    buffer_3 = malloc((MAX_PROGRAM_SIZE) * sizeof(char*));

    pthread_t input_t, line_separator_t, plus_signs_t, output_t;
    // Create the threads
    pthread_create(&input_t, NULL, get_input, NULL);
    pthread_create(&line_separator_t, NULL, get_line_separator, NULL);
    pthread_create(&plus_signs_t, NULL, get_plus_signs, NULL);
    pthread_create(&output_t, NULL, thread_output, NULL);

    // Wait for the threads to terminate
    pthread_join(input_t, NULL);
    pthread_join(line_separator_t, NULL);
    pthread_join(plus_signs_t, NULL);
    pthread_join(output_t, NULL);

    // free buffers and lines used in input transformation
    free_mem(buffer_1, buffer_2, buffer_3);
    return 0;
}