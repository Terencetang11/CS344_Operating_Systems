/* Name : Terence Tang
*  Course : CS344 - Operating Systems
*  Date : May 3, 2021
*  Assignment #3: Smallsh
*  Description:  Header file for defining the small shell library of functions 
*                used for this assignment.
*/


// Declaring Constants
#define MAX_BUFFER_SIZE 2048
#define ARG_BUFFER_SIZE 512
#define MAX_BG_PROCS 100

/* struct for input command information */
struct command_input
{
    char *command;
    char **args;
    int input_redirect;
    int output_redirect;
    int background_flag;
};

/* 
* Prompts user for input and reads line from standard input via fgets().
* Returns pointer to stored input.
*/
char* read_line();

/* 
* Parses user input and reads each token into a command_input struct which 
* stores the top level command of input and the follow-up arguments.
* Returns command_input struct
*/
struct command_input* parse_line(char *line);

/* 
* Frees all dynamically allocated memory elements as part of the current command input
* Includes freeing memory for input line, the command_input struct, and it's args array
*/
int free_mem(char *line, struct command_input *currCommand);

/* 
* Function to replace a string with another value, all within another string (haystack)
*/
char* replace_string(const char* string, const char* old_str, const char* new_str);

/* 
* Executes other commands using the exec family of funcs
*/
void exec_other_commands(struct command_input *currCommand);

int built_in_router(struct command_input *currCommand);

int cmd_cd(char **args);

int cmd_exit();

int cmd_status(int status);

void check_redirects(struct command_input *currCommand);

void handle_SIGTSTP();

void set_default_signal_handlers();

void set_child_signal_handlers(int process_bg_flag);
