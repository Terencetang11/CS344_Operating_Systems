# Assignment 3: smallsh
 	By: Terence Tang
 	tangte@oregonstate.edu
 	CS 344 Operating Systems
 	5/5/2020


    Included files:
    - smallsh.c 
    - shell_input.c
    - shell_built_in_funcs.c
    - shell_executing_commands.c
    - shell_signal_handling.c
    - smallsh.h


    Simple os shell application written in C. Features many common and helpful shell functions:
 	- Command prompt for entering and running command lines
	- Handling comments and blank lines.  Comments begin with a '#' char
	- Expansion of variable.  The $$ var is expanded to shell pid
	- Built-in commands for cd, exit, and status functions
	- Executes other commands via fork(), execvp(), and waitpid()
	- Input and output redirection
	- Supports running foreground and background processes
	- Custom signal handlers for catching SIGINT and SIGTSTP


## Installation
    Instructions on how to compile Assignment #3 Smallsh:
	- Terminal Command for compiling -
    gcc --std=c99 -o smallsh smallsh.c shell_input_output.c shell_executing_commands.c  shell_built_in_funcs.c shell_signal_handling.c

    - Terminal Command for running resulting executable -
    ./smallsh