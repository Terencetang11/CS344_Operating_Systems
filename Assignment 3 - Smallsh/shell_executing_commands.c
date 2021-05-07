/* 
*  Name : Terence Tang
*  Course : CS344 - Operating Systems
*  Date : May 3, 2021
*  Assignment #3: Smallsh
*  Description:  Simple shell implementation featuring many common and helpful shell functionality, e.g. changing directories
*                running executables with given list of arguments, redirecting input/output, etc.
*/

// compiled using gcc option --std=c99

#define _GNU_SOURCE
#include <fcntl.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include "smallsh.h"

// Declare Global Variables
extern int background_boolean;              // if true(1), then bg process cmds with '&' are allowed
extern int background_processes[];
extern int background_process_counter;
extern int status;                          // tracks foreground statuses

int built_in_router(struct command_input *currCommand)
{
    if (strcmp(currCommand->command, "\n") == 0 || currCommand->args[0][0] == '#')
    {
        // ignores comments and empty input lines
    }
    else if (strcmp(currCommand->command,"cd") == 0)
    {
        cmd_cd(currCommand->args);
    } 
    else if (strcmp(currCommand->command,"exit") == 0)
    {
        // printf("exit command used\n");
        // fflush(stdout);
        cmd_exit();
    }
    else if (strcmp(currCommand->command,"status") == 0)
    {
        // printf("status command used\n");
        // fflush(stdout);
        cmd_status(status);
    }
    else 
    {
        exec_other_commands(currCommand);
    }
    return 1;
}


void exec_other_commands(struct command_input *currCommand)
{
    int childStatus;

	// Fork a new process
	pid_t childPid = fork();

	switch(childPid){
	case -1:
		perror("fork()\n");
		exit(1);
		break;
	case 0:
		// In the child process
		// printf("CHILD(%d) running %s command\n", getpid(), currCommand->command);
        // fflush(stdout);
        set_child_signal_handlers(currCommand->background_flag);  // set signal handlers for child processes
        check_redirects(currCommand);

        if (execvp(currCommand->args[0], currCommand->args) == -1) {
            perror(currCommand->args[0]);
            exit(1);
        }
		break;
	default:
		// In the parent process
        // check for if child is bg process or not
        if(currCommand->background_flag == 1 && background_boolean == 1) 
        {

            // if background process, proceed with no hang and print child process pid
            waitpid(childPid, &childStatus, WNOHANG);
            printf("background pid is %d\n", childPid);
            fflush(stdout);

            // store bg child pig for tracking
            background_processes[background_process_counter] = childPid;
            background_process_counter++;
        }
        // else handle child as foreground process
        else 
        {
            // if foreground process, wait for child termination
		    childPid = waitpid(childPid, &status, 0);
            // printf("PARENT(%d): child(%d) terminated. Exiting\n", getpid(), childPid);
            // printf("the exit status of the child is as follows: %d\n", childStatus);babbabab
            fflush(stdout);

            // need to check for termination of bg child process
	    }

		// Wait for child's termination
		while((childPid = waitpid(-1, &childStatus, WNOHANG)) > 0)
        {
            printf("background pid %d is done: ", childPid);
            fflush(stdout);
            cmd_status(childStatus); 

            // clear bg pid from process array
            for (int i = 0; i < background_process_counter; i++)
            {
                if (background_processes[i] == childPid)
                {
                    background_processes[i] = 0;
                }
            }
        }
	}
}

void check_redirects(struct command_input *currCommand)
{
    int dup_fd; // var for catching dup2() fd return

    // if background flag set - set stdin and stdout to '/dev/null' file first
    if(currCommand->background_flag == 1 && background_boolean == 1)
    {
        char bg_redirect[] = "/dev/null";
        int sourceFD = open(bg_redirect, O_RDONLY);
        if (sourceFD == -1) { 
            perror("source open()"); 
            exit(1); 
        }

        // Redirect stdin to source file
        dup_fd = dup2(sourceFD, 0);
        if (dup_fd == -1) { 
            perror("source dup2()"); 
            exit(1); 
        }
        close(sourceFD);

        int targetFD = open(bg_redirect, O_WRONLY | O_CREAT | O_TRUNC, 0644);
        if (targetFD == -1) { 
            perror("target open()"); 
            exit(1); 
        }
        // Redirect stdout to target file
        dup_fd = dup2(targetFD, 1);
        if (dup_fd == -1) { 
            perror("target dup2()"); 
            exit(1);
        }
        close(targetFD);
    }

    // Source redirect flag set, open source file and set
    if(currCommand->input_redirect != -1)
    {
        int sourceFD = open(currCommand->args[currCommand->input_redirect + 1], O_RDONLY);
        if (sourceFD == -1) { 
            // perror("source open()"); 
            fprintf(stderr, "cannot open %s for input\n", currCommand->args[currCommand->input_redirect + 1]);
            fflush(stdout);
            exit(1); 
        }
        // remove redirect symbol from args list
        currCommand->args[currCommand->input_redirect] = NULL;

        // Redirect stdin to source file
        dup_fd = dup2(sourceFD, 0);
        if (dup_fd == -1) { 
            perror("source dup2()"); 
            exit(1); 
        }
        close(sourceFD);
    }

    // Target redirect flag set, open target file and set
    if(currCommand->output_redirect != -1)
    {
        // Open target file
        int targetFD = open(currCommand->args[currCommand->output_redirect + 1], O_WRONLY | O_CREAT | O_TRUNC, 0644);
        if (targetFD == -1) { 
            fprintf(stderr, "cannot open %s for out\n", currCommand->args[currCommand->output_redirect + 1]);
            fflush(stdout);
            // perror("target open()"); 
            exit(1); 
        }

        // remove redirect symbol from args list
        currCommand->args[currCommand->output_redirect] = NULL;
    
        // Redirect stdout to target file
        dup_fd = dup2(targetFD, 1);
        if (dup_fd == -1) { 
            perror("target dup2()"); 
            exit(1); 
        }
        close(targetFD);
    }
}


