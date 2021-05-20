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

// Thread 1 variables - Buffers, producer and consumer indexes, mutex and cond vars.
extern char **buffer_1;                                         // Declare buffer to store lines of str input
extern int count_1;                    // Number of items in the buffer
extern int prod_idx_1;                                     // Index where the input thread will put the next item
extern int con_idx_1;                                      // Index where the replace LS thread will pick up the next item
extern pthread_mutex_t mutex_1;    // Initialize the mutex for buffer 1
extern pthread_cond_t full_1;       // Initialize the condition variable for buffer 1


/*
* Put an item in buff_1
*/
void put_buff_1(char *line)
{
    pthread_mutex_lock(&mutex_1);         // Lock the mutex before putting the item in the buffer
    buffer_1[prod_idx_1] = line;          // Put the item in the buffer
    prod_idx_1 = prod_idx_1 + 1;          // Increment the index where the next item will be put.
    count_1++;
    pthread_cond_signal(&full_1);         // Signal to the consumer that the buffer is no longer empty
    pthread_mutex_unlock(&mutex_1);       // Unlock the mutex
}

/* 
* Gets line of user input, either from stdin or file redirect
*/
char *read_input_line()
{   
    // set up variables - getline will allocate memory for line input
    char *line = calloc(MAX_INPUT_BUFFER_SIZE + 1,sizeof(char));

    if (fgets(line, MAX_INPUT_BUFFER_SIZE, stdin) == NULL){             // check user input via stdin for error
        if (feof(stdin)) {                                              // recieved an EOF error, program continues
            stop_input = true;
            return NULL;                                                
        } else  {                                                       // received an error and needs to quit
            perror("read_line");
            exit(EXIT_FAILURE);                                         
        }
    }
    else if (stop_input == true || strcmp(line, END_MARKER) == 0)       // if no error, check STOP signal has been sent
    {
        stop_input = true;
        return NULL;                                                    // if stop already signaled, return null
    } 
    return line;                                                        // else return stdin line
}

/*
* Function that the input thread will run.
* Get input from the user.
* Put the item in the buffer shared with the replace_line_separator thread.
*/
void *get_input(void *args)
{
    for (int i = 0; i < MAX_PROGRAM_SIZE; i++)      // reads lines for the max # of defined line inputs for the program
    {
        char *line = read_input_line();
        put_buff_1(line);

        if (stop_input == true && line == NULL)
        {
            return NULL;
        }
    }
    return NULL;
}

