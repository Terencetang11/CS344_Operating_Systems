/* 
*  Name : Terence Tang
*  Course : CS344 - Operating Systems
*  Date : May 16 2021
*  Assignment #4: Multithreaded Producers and Consumers Pipeline
*  Description:  Header file for defining the multithreaded library of functions showcasing 
*                how to handle producers and consumers pipelines, mutex, and cond vars for this assignment
*/


// Declare Constants
#define MAX_INPUT_BUFFER_SIZE 1000
#define MAX_PROGRAM_SIZE 50
#define ASCII_LOWER 32
#define ASCII_UPPER 126
#define MAX_OUTPUT_BUFFER_SIZE 80
#define END_MARKER "STOP\n"

// Struct Declarations
struct output_line
{
    char *output;
    struct output_line *next;
};

// Function Declarations
int free_mem(char **buffer_1, char **buffer_2, char **buffer_3);
void put_buff_1(char *line);
char *read_line();
void *get_input(void *args);
char *get_buff_1();
void put_buff_2(char *line);
void replace_line_separator(char *input_line);
void *get_line_separator(void *args);

char *get_buff_2();
void put_buff_3(char *line);
void replace_plus_signs(char *input_line);
void *get_plus_signs(void *args);

char *get_buff_3();
void print_output(char *input_line, char *output_line);
void *thread_output(void *args);
