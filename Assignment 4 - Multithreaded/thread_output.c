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


// Declare Global Variables
extern int stop_input;     // bool for if STOP has been correctly entered into stdin

// Thread 3 variables - Buffers, producer and consumer indexes, mutex and cond vars.
extern char **buffer_3;                 // Declare buffer to store lines of str
extern int count_3;                     // Number of items in the buffer
extern int con_idx_3;                   // Index where the replace LS thread will read the next item
extern pthread_mutex_t mutex_3;         // Initialize the mutex for buffer 3
extern pthread_cond_t full_3;           // Initialize the condition variable for buffer 3


/*
Get the next item from buffer_3
*/
char *get_buff_3()
{
    // set up variables - getline will allocate memory for line input
    char *line = calloc(MAX_INPUT_BUFFER_SIZE + 1, sizeof(char));

    pthread_mutex_lock(&mutex_3);               // Lock the mutex before checking if the buffer has data
    while (count_3 == 0)
    {
        pthread_cond_wait(&full_3, &mutex_3);   // Buffer is empty. Wait for the producer to signal that the buffer has data
    }
    if (buffer_3[con_idx_3] != NULL)
    {
        strcpy(line, buffer_3[con_idx_3]);      // Get line from buffer
    }
    else {
        line = NULL;
    }
    
    con_idx_3 = con_idx_3 + 1;                  // Increment the index from which the item will be picked up
    count_3--;
    pthread_mutex_unlock(&mutex_3);             // Unlock the mutex
    return line;
}


void print_output(char *input_line, char *output_line)
{
    int i = strlen(output_line);

    // if current output line is at 80 create new output line
    while (*input_line) {            
        if (strlen(output_line) == MAX_OUTPUT_BUFFER_SIZE)
        {
            output_line[MAX_OUTPUT_BUFFER_SIZE] = '\0';
            printf("%s\n", output_line);
            memset(output_line, '\0', strlen(output_line));
            i = 0;
        }
        else output_line[i++] = *input_line++;   
    }
}

/*
 Function that the plus signs thread will run. 
 Consume an line from the buffer shared with the line separator thread.
 Replace the ++ str with carets for the line.
 Produce an line in the buffer shared with the output thread.
*/
void *thread_output(void *args)
{
    char *output_line = calloc(MAX_OUTPUT_BUFFER_SIZE + 1, sizeof(char));
    for (int i = 0; i < MAX_PROGRAM_SIZE; i++)          // reads lines for the max # of defined line inputs for the program
    {
        char *line = get_buff_3();
        if (line != NULL)
        {
            print_output(line, output_line);
        }
        free(line);

        if (stop_input == true && line == NULL)
        {
            free(output_line);
            return NULL;
        }
    }
    free(output_line);
    return NULL;
}