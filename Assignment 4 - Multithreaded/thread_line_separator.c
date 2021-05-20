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


// Thread 1 variables - Buffers, producer and consumer indexes, mutex and cond vars.
extern char **buffer_1;                 // Declare buffer to store lines of str input
extern int count_1;                     // Number of items in the buffer
extern int con_idx_1;                   // Index where the line separator thread will read the next item
extern pthread_mutex_t mutex_1;         // Initialize the mutex for buffer 1
extern pthread_cond_t full_1;           // Initialize the condition variable for buffer 1

// Thread 2 variables - Buffers, producer and consumer indexes, mutex and cond vars.
extern char **buffer_2;                  // Declare buffer to store lines of str
extern int count_2;                     // Number of items in the buffer
extern int prod_idx_2;                  // Index where the line separator thread will put the next item
extern pthread_mutex_t mutex_2;         // Initialize the mutex for buffer 2
extern pthread_cond_t full_2;           // Initialize the condition variable for buffer 2

extern char **input_lines;

/*
* Get the next item from buffer_1
*/
char *get_buff_1()
{
    // set up variables - getline will allocate memory for line input
    char *line = calloc(MAX_INPUT_BUFFER_SIZE + 1, sizeof(char));

    pthread_mutex_lock(&mutex_1);               // Lock the mutex before checking if the buffer has data
    while (count_1 == 0)
    {
        pthread_cond_wait(&full_1, &mutex_1);   // Buffer is empty. Wait for the producer to signal that the buffer has data
    }
    if (buffer_1[con_idx_1] != NULL)
    {
        strcpy(line, buffer_1[con_idx_1]);      // Get line from buffer
    }
    else {
        line = NULL;
    }
    

    con_idx_1 = con_idx_1 + 1;                  // Increment the index from which the item will be picked up
    count_1--;                                  // Decrement count of buffer_1 items
    pthread_mutex_unlock(&mutex_1);             // Unlock the mutex
    return line;
}

/*
* Put an item in buffer_2
*/
void put_buff_2(char *line)
{
    pthread_mutex_lock(&mutex_2);               // Lock the mutex before putting the item in the buffer
    buffer_2[prod_idx_2] = line;                // Put the item in the buffer
    prod_idx_2 = prod_idx_2 + 1;                // Increment the index where the next item will be put.
    count_2++;
    pthread_cond_signal(&full_2);               // Signal to the consumer that the buffer is no longer empty
    pthread_mutex_unlock(&mutex_2);             // Unlock the mutex
}

/* 
* Takes a line of input text and replaces all line separator chars with spaces
*/
void replace_line_separator(char *line)
{   
    int i;
    for(i = 0; line[i] != '\0'; i++)            // scans each char in line for line separator
    {
        if (line[i] == '\n')
        {
            line[i] = ' ';                      // replaces line separator with space
        }
    }
}

/*
* Function that the line separator thread will run. 
* Consume an line from the buffer shared with the input thread.
* Replace the line separators for the line.
* Produce an line in the buffer shared with the plus sign thread.
*/
void *get_line_separator(void *args)
{
    for (int i = 0; i < MAX_PROGRAM_SIZE; i++)      // reads lines for the max # of defined line inputs for the program
    {
        char *line = get_buff_1();
        if (line != NULL)
        {
            replace_line_separator(line);
        }
        put_buff_2(line);
    }
    return NULL;
}

