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
extern int background_boolean;     // if true(1), then bg processes entered with '&' are allowed

void handle_SIGTSTP(int signo)
{
	char* updateMsg;		// String of the message to write to stdout
	// char* promptMessage = ": "; // Also write ': ' since for this case getCommands() won't print it
	switch(background_boolean) 
	{
		case 0:
			updateMsg = "\nExiting foreground-only mode\n";
			background_boolean = 1;
			break;
		case 1:
			updateMsg = "\nEntering foreground-only mode (& is now ignored)\n";
			background_boolean = 0;
			break;
		default:
			updateMsg = "\nError: background processes boolean is not set\n";
			background_boolean = 1;
	}
	// Must use reentrant function for custom signal handlers
	write(1, updateMsg, strlen(updateMsg));
	write(1, ": ", 2);
	fflush(stdout);
}

// void handle_SIGINT(int signo)
// {
// 	kill()
// 	cmd_status(WTERMSIG(signo));
// 	write(1, ": ", 2);
// 	fflush(stdout);
// }

void set_default_signal_handlers()
{
	struct sigaction SIGINT_action = {0}, SIGTSTP_action = {0};		// initialize sig struct for SIGINT and SIGTSTP

	// Set SIGINT signal handler
	SIGINT_action.sa_handler = SIG_IGN;				// set handler to ignore action
	sigfillset(&SIGINT_action.sa_mask);				// set mask to block all other signals
	SIGINT_action.sa_flags = 0;
    sigaction(SIGINT, &SIGINT_action, NULL);		// install handler for SIGINT signal

	// Set SIGTSTP signal handler
	SIGTSTP_action.sa_handler = handle_SIGTSTP;  	// set handler to ignore action
	sigfillset(&SIGTSTP_action.sa_mask);			// set mask to block all other signals
    SIGTSTP_action.sa_flags = SA_RESTART; 			// flag ensures smooth resumption of library functions after write
    sigaction(SIGTSTP, &SIGTSTP_action, NULL);		// install handler for SIGINT signal
}


void set_child_signal_handlers(int process_bg_flag)
{
	
	// Set SIGTSTP signal handler for both foreground and background child processes
	struct sigaction SIGTSTP_action = {0};			// initialize sig struct for SIGINT and SIGTSTP
	SIGTSTP_action.sa_handler = SIG_IGN;  			// set handler to ignore action
	sigfillset(&SIGTSTP_action.sa_mask);			// set mask to block all other signals
	SIGTSTP_action.sa_flags = 0;
	sigaction(SIGTSTP, &SIGTSTP_action, NULL);		// install handler for SIGINT signal

	// Set SIGINT handler for foreground child processes
	if (process_bg_flag == 0)
	{
		struct sigaction SIGINT_action = {0};		// initialize sig struct for SIGINT and SIGTSTP

		// Set SIGINT signal handler
		SIGINT_action.sa_handler = SIG_DFL;			// set handler to ignore action
		sigfillset(&SIGINT_action.sa_mask);			// set mask to block all other signals
		SIGINT_action.sa_flags = 0;
		sigaction(SIGINT, &SIGINT_action, NULL);	// install handler for SIGINT signal
	}
}
