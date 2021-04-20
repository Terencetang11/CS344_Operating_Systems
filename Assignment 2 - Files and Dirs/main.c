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
#include <time.h>
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
    // // checks to see if a filepath was provided for the input movie data
    // if (argc < 2)
    // {
    //     printf("You must provide the name of the file to process\n");
    //     printf("Example usage: ./movies_sample_1.csv\n");
    //     return EXIT_FAILURE;
    // }

    // set up user input variables
    int programChoice;
    int fileChoice;
    int fileCheck;
    int directoryCheck;
    int lengthFileName = 256;
    char fileName[lengthFileName];
    char filePath[lengthFileName];
    char directoryName[20];
    char directoryPath[22];
    char *onid = "tangte";


    // Prints user w/ program instructions and prompts for user selection
    while (programChoice != 2)
    {
        printInstructions(&programChoice);

        // print instructions
        if (programChoice == 1)
        {
            printFileProcessingInstructions(&fileChoice);

            if (fileChoice != 1 && fileChoice != 2 && fileChoice !=3)
            {
                printf("You entered an incorrect choice. Try again.\n");
            } 
            else
            {
                // run program and pass the supplied choice int to program
                fileCheck = getFileName(&fileChoice, fileName, lengthFileName);
                while (fileCheck != 0 && fileChoice == 3)
                {
                    printFileProcessingInstructions(&fileChoice);
                    fileCheck = getFileName(&fileChoice, fileName, lengthFileName);
                }
                if (fileCheck == 0)
                {
                    // process file
                    printf("Now processing the chosen file named %s\n", fileName);
                    strcpy(filePath, "./");
                    strcat(filePath, fileName);
                    struct movie *list = processFile(filePath);

                    // get random naming for new directory and make directory

                    do {
                        setDirectoryName(directoryName, onid);
                        directoryCheck = mkdir(directoryName, 0750);
                    } while (directoryCheck != 0);
                    strcpy(directoryPath,"./");
                    strcat(directoryPath,directoryName);

                    printf("directory successfully made: %s\n", directoryName);
                    // write files  
                    writeMoviesToDirectory(list, directoryPath);               


                }


                printf("\n");        
            }
        }
        else if (programChoice != 2) printf("You entered an incorrect choice. Try again.\n\n");
    }
    return EXIT_SUCCESS;
}
