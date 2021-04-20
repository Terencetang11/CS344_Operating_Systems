/* 
*  Name : Terence Tang
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
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <time.h> // not needed
#include <unistd.h>
#include "file_and_directory_processing.h"


/*
*   Process the file provided as an argument to the program to
*   create a linked list of student structs and print out the list.
*   Compile the program as follows:
*       gcc --std=gnu99 -o students main.c
*/

int main()
{
    srandom(time(0));
    int num = (random() % (9 - 0 + 1)) + 0;    
    printf("size of dirNum: %lu\n", sizeof(num));  
    printf("randomly generated number: %d\n", num);
    return EXIT_SUCCESS;
}
