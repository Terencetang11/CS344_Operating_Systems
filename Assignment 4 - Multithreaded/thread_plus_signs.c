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

// Thread 2 variables - Buffers, producer and consumer indexes, mutex and cond vars.
extern char **buffer_2;                 // Declare buffer to store lines of str
extern int count_2;                     // Number of items in the buffer
extern int con_idx_2;                   // Index where the plus signs thread will read the next item
extern pthread_mutex_t mutex_2;         // Initialize the mutex for buffer 2
extern pthread_cond_t full_2;           // Initialize the condition variable for buffer 2

// Thread 3 variables - Buffers, producer and consumer indexes, mutex and cond vars.
extern char **buffer_3;                 // Declare buffer to store lines of str
extern int count_3;                     // Number of items in the buffer
extern int prod_idx_3;                  // Index where the plus signs thread will put the next item
extern pthread_mutex_t mutex_3;         // Initialize the mutex for buffer 3
extern pthread_cond_t full_3;           // Initialize the condition variable for buffer 3

extern char **input_lines;


/*
Get the next item from buffer_2
*/
char *get_buff_2()
{
    // set up variables - getline will allocate memory for line input
    char *line = calloc(MAX_INPUT_BUFFER_SIZE + 1, sizeof(char));

    pthread_mutex_lock(&mutex_2);               // Lock the mutex before checking if the buffer has data
    while (count_2 == 0)
    {
        pthread_cond_wait(&full_2, &mutex_2);   // Buffer is empty. Wait for the producer to signal that the buffer has data
    }
    if (buffer_2[con_idx_2] != NULL)
    {
        strcpy(line, buffer_2[con_idx_2]);      // Get line from buffer
    }
    else {
        line = NULL;
    }
    

    con_idx_2 = con_idx_2 + 1;                  // Increment the index from which the item will be picked up
count_2--;                                      // Decrement count of buffer_2 items
    pthread_mutex_unlock(&mutex_2);             // Unlock the mutex
    return line;
}

/*
 Put an item in buffer_3
*/
void put_buff_3(char *line)
{
    pthread_mutex_lock(&mutex_3);               // Lock the mutex before putting the item in the buffer
    buffer_3[prod_idx_3] = line;                // Put the item in the buffer
    prod_idx_3 = prod_idx_3 + 1;                // Increment the index where the next item will be put.
    count_3++;
    pthread_cond_signal(&full_3);               // Signal to the consumer that the buffer is no longer empty
    pthread_mutex_unlock(&mutex_3);             // Unlock the mutex
}


/* 
* Function to replace a substring with another value, all within another string (haystack)
*/
void replace_plus_signs(char *input_line)
{
    char *result;
    char *lineptr = input_line;       // retains ptr to beginning of line for copying results
    int i, count = 0;
    const char *plus_signs = "++";
    int plusSignsLen = strlen(plus_signs);
    const char *caret = "^";
    int caretLen = strlen(caret);
  
    // Counting the number of times the old string occurs in string (before null char)
    for (i = 0; input_line[i] != '\0'; i++) {
        if (strstr(&input_line[i], plus_signs) == &input_line[i]) {
            count++;
  
            // jumps to index past old str, tracks string's len without the old str chars
            i += plusSignsLen - 1;
        }
    }
  
    // Making new string of enough length
    result = (char*)malloc(i + count * (caretLen - plusSignsLen) + 1);
  
    i = 0;
    while (*input_line) {
        // compare the substring with the result and replace w/ new_str (pid)
        if (strstr(input_line, plus_signs) == input_line) {
            strcpy(&result[i], caret);
            i += caretLen;
            input_line += plusSignsLen;
        }
        // else copy over string char by char
        else result[i++] = *input_line++;
    }

    // adds null char and returns new string
    result[i] = '\0';
    strcpy(lineptr, result);
    free(result);
}

/*
 Function that the plus signs thread will run. 
 Consume an line from the buffer shared with the line separator thread.
 Replace the ++ str with carets for the line.
 Produce an line in the buffer shared with the output thread.
*/
void *get_plus_signs(void *args)
{
    for (int i = 0; i < MAX_PROGRAM_SIZE; i++)      // reads lines for the max # of defined line inputs for the program
    {
        char *line = get_buff_2();
        if (line != NULL)
        {
            replace_plus_signs(line);
        }
        put_buff_3(line);

        if (stop_input == true && line == NULL)
        {
            return NULL;
        }
    }
    return NULL;
}