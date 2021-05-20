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

// Declare Constants
#define MAX_INPUT_BUFFER_SIZE 1000
#define MAX_PROGRAM_SIZE 49
#define ASCII_LOWER 32
#define ASCII_UPPER 126
#define MAX_OUTPUT_BUFFER_SIZE 80



// Declare Global Variables
bool stop_input = false;

/* struct for output_line */
struct output_line
{
    char *output;
    struct output_line *next;
};


/* 
* Frees all dynamically allocated memory elements as part of the current command input
* Includes freeing memory for input line, the command_input struct, and it's args array
*/
int free_mem(char **input_lines, char *output_line)
{
    // resets command line input datastructure
    int index = 0;
    char* line = input_lines[index];
    while (line != NULL)
    {
        free(line);
        index++;
        line = input_lines[index];
    }

    free(output_line);
    return 1;
}

/* 
* Prompts user for input and reads line from standard input via get line.
* Returns pointer to stored input.
*/
char* read_line()
{   
    // set up variables - getline will allocate memory for lineptr and set buffersize
    char *line = malloc((MAX_INPUT_BUFFER_SIZE + 1) * sizeof(char));
    size_t buffersize = 0;

    // get user input via commandline
    if (fgets(line, MAX_INPUT_BUFFER_SIZE, stdin) == NULL){
        // if getline returns in error, check for EOF and or error code and exit
        if (feof(stdin)) {
            return NULL;  // We recieved an EOF
        } else  {
            perror("read_line");
            exit(EXIT_FAILURE);
        }
    }
    else if (strcmp(line, "STOP\n") == 0)
    {
        stop_input = true;
    }
    // printf("%d, %s", strcmp(line, "STOP\n"), line);
    return line;
}

/* 
* Prompts user for input and reads line from standard input via get line.
* Returns pointer to stored input.
*/
void replace_line_separator(char *input_line)
{   
    for(int i = 0; input_line[i] != '\0'; i++)
    {
        if (input_line[i] == '\n')
        {
            input_line[i] = ' ';
        }
    }
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

struct output_line *initialize_output_struct()
{
    struct output_line *currOutput = malloc(sizeof(struct output_line));
    currOutput->output = calloc(MAX_OUTPUT_BUFFER_SIZE + 1, sizeof(char));
    currOutput->next = NULL;
    return currOutput;
}


int main()
{
    // set up program resources - **input_lines is shared resource across threads
    char **input_lines = malloc((MAX_PROGRAM_SIZE) * sizeof(char*));
    char *output_line = calloc(MAX_OUTPUT_BUFFER_SIZE + 1, sizeof(char));

    int count = 0;
    while(stop_input != true && count <= 45)
    {
        input_lines[count] = read_line();
        // replace new line chars
        if (input_lines[count] != NULL)
        {
            replace_line_separator(input_lines[count]);
            replace_plus_signs(input_lines[count]);
            print_output(input_lines[count], output_line);
            // push into 80 char segments
            // print segments
            // printf("here is what was read %s", input_lines[i]);
        }
        count++;
    }

    free_mem(input_lines, output_line);
    return 0;
}