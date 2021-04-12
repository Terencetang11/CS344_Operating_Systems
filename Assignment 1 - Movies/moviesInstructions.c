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
#include "movies.h"

/*
* Print the instructions for what details the program can provide
*/
void printInstructions()
{
    printf("\n");
    printf("1. Show movies released in the specified year\n");
    printf("2. Show highest rated movie for each year\n");
    printf("3. Show the title and year of release of all movies in a specific language\n");
    printf("4. Exit from the program\n");
    printf("\n");
}

/*
* Prompts user for input regarding which search feature they 
* would like to utilize on the given list of movies
*/
int getUserFeatureChoice()
{
    printInstructions();
    int choice;
    printf("Enter a choice from 1 to 4: ");
    scanf("%d", &choice);
    return choice;
}