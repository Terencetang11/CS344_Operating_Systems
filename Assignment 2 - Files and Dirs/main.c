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

    // set up user input and file path / directory name variables for tracking file / directory paths
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

    // Prints program instructions and prompts for user selection
    while (programChoice != 2)
    {
        printInstructions(&programChoice);

        // print file processing instructions and prompts user for selection
        if (programChoice == 1)
        {
            printFileProcessingInstructions(&fileChoice);
            
            // checks for invalid user inputs
            if (fileChoice != 1 && fileChoice != 2 && fileChoice !=3)
            {
                printf("You entered an incorrect choice. Try again.\n");
            } 
            else
            {
                // run program and pass the supplied choice int to program
                fileCheck = getFileName(&fileChoice, fileName, lengthFileName);

                // if user selected to provide custom input, and no file was found, re-prompts user
                // for file processing selection
                while (fileCheck != 0 && fileChoice == 3)
                {
                    printFileProcessingInstructions(&fileChoice);
                    fileCheck = getFileName(&fileChoice, fileName, lengthFileName);
                }

                // if file is found, processes file data and creates new directory w/ new file outputs
                if (fileCheck == 0)
                {
                    // process file and creates a movie linked list object
                    printf("Now processing the chosen file named %s\n", fileName);
                    strcpy(filePath, "./");
                    // converts file name to an appropriate filepath with directory and folder structure
                    strcat(filePath, fileName); 
                    struct movie *list = processFile(filePath);

                    // get naming for new directory w/ random int and make directory
                    // if directory already exists, tries again
                    do {
                        setDirectoryName(directoryName, onid);
                        directoryCheck = mkdir(directoryName, 0750);
                    } while (directoryCheck != 0);
                    // stores directoryPath from directoryName
                    strcpy(directoryPath,"./");
                    strcat(directoryPath,directoryName);

                    // write movie files to new directory
                    writeMoviesToDirectory(list, directoryPath);  
                    printf("Created directory with name %s\n", directoryName);
                }
                printf("\n");        
            }
        }
        // if top-level selection is neither 1 nor 2, prints invalid input message
        else if (programChoice != 2) printf("You entered an incorrect choice. Try again.\n\n");
    }

    // if top-level selection is 2, exits program successfully
    return EXIT_SUCCESS;
}
