/* 
*  Name : Terence Tang
*  Course : CS344 - Operating Systems
*  Date : Apr 12, 2021
*  Assignment #2: Files and Directories
*  Description:  C Program built to practice reading and writing files and directories in C via system calls.
*                Main program built to scan current directory for files given specific prefix and extension criteria. 
*                Gives the user the ability to scan for the largest or smallest files or to provide custom file name input.
*                Program then creates a new unique folder directory and prints file's movie contents into new files.
*                New files are created for each year a movie was released per the input file.
*/


// compiled using gcc option --std=c99

#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "file_and_directory_processing.h"

/*
* Print the instructions for what details the program can provide
*/

void printInstructions(int *programChoice)
{
    // set up variables
    printf("1. Select file to process\n");
    printf("2. Exit the program\n\n");
    printf("Enter a choice 1 or 2: ");
    scanf("%d", programChoice);

}

/*
* Print the instructions for what type of file processing the program can provide
*/
void printFileProcessingInstructions(int *fileChoice)
{
    printf("\nWhich file you want to process?\n");
    printf("Enter 1 to pick the largest file\n");
    printf("Enter 2 to pick the smallest file\n");
    printf("Enter 3 to specify the name of a file\n\n");
    printf("Enter a choice from 1 to 3: ");
    scanf("%d", fileChoice);
}

