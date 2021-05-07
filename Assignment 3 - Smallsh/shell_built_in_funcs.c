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
extern int background_processes[];
extern int background_process_counter;
extern int status;                          // tracks foreground statuses

int cmd_cd(char **args)
{
    char cwd[1024];     // var to store directory string

    if (args[1] == NULL) 
    {
        if (chdir(getenv("HOME")) != 0) 
        {
            perror("smallsh");
        } 
        // getcwd(cwd, sizeof(cwd));
        // printf("%s\n", cwd);
        // fflush(stdout);
    } 
    else 
    {
        if (chdir(args[1]) != 0) {
            perror("smallsh");
        } 
        // getcwd(cwd, sizeof(cwd));
        // printf("%s\n", cwd);
        // fflush(stdout);
    }

    // print new directory
    getcwd(cwd, sizeof(cwd));
    printf("%s\n", cwd);
    fflush(stdout);
    return 1;
}

int cmd_status(int status)
{
    if(WIFEXITED(status))
    {
        printf("exit value %d\n", WEXITSTATUS(status));
    }
    else
    {
        printf("terminated by signal %d\n", WTERMSIG(status));
    }
    return 1;
}

int cmd_exit()
{
    for (int i = 0; i < background_process_counter; i++)
    {
        kill(background_processes[i], SIGTERM);
    }

    exit(0);
    // need to write condition for closing child processes
}

