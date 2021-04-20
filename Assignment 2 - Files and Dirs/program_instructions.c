/* Name : Terence Tang
*  Course : CS344 - Operating Systems
*  Date : Apr 12, 2021
*  Assignment #1: Movies
*  Description:  C Program built to practice using system calls for input, output, and reading / manipulating files.
*                Main program built to consume a given file of movies, releaes years, languages, ratings and then
*                allows user to prompt for additional details, e.g. movies released in a specific year, or language.
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

