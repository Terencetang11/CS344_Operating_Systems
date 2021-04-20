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
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <time.h> // not needed
#include <unistd.h>
#include "file_and_directory_processing.h"

#define PREFIX "movies_"
#define EXTENSION ".csv"


/*
* Parses user input on which file processing feature they 
* would like to utilize on the given list of movies
*/
int getFileName(int *fileChoice, char *fileName, int lengthFileName)
{
    int fileCheck = 1;
    if (*fileChoice == 1)
    {
        fileCheck = getLargestMovieCSV(fileName, lengthFileName);
    }
    else if (*fileChoice == 2)
    {
        fileCheck =  getSmallestMovieCSV(fileName, lengthFileName);
    }
    else if(*fileChoice == 3)
    {
        fileCheck =  getUserInputFile(fileName, lengthFileName);
    }
    return fileCheck;
}

int getLargestMovieCSV(char *fileName, int lengthFileName)
{
    // set up directory traverse variables and initialize currDir
    DIR *currDir = opendir(".");
    struct dirent *aDir;
    int fileSize;
    struct stat dirStat;
    int i = 0;
    int fileCheck = 1;

    // Go through all the entries
    while ((aDir = readdir(currDir)) != NULL)
    {
        if (strncmp(PREFIX, aDir->d_name, strlen(PREFIX)) == 0 && strcmp(EXTENSION, strrchr(aDir->d_name,'.')) == 0)
        {
            fileCheck = 0;

            // Get meta-data for the current entry
            stat(aDir->d_name, &dirStat);  
            
            // Use the difftime function to get the time difference between the current value of lastModifTime and the st_mtime value of the directory entry
            if(i == 0 || dirStat.st_size > fileSize){
                fileSize = dirStat.st_size;
                memset(fileName, '\0', lengthFileName);
                strcpy(fileName, aDir->d_name);
                }
            i++;
        }
    }
    // Close the directory
    closedir(currDir);
    if (fileCheck == 1) printf("No files that begin with prefix 'movies_' and has a '.csv' extension exists");
    return fileCheck;
}

int getSmallestMovieCSV(char *fileName, int lengthFileName)
{
    // set up directory traverse variables and initialize currDir
    DIR *currDir = opendir(".");
    struct dirent *aDir;
    int fileSize;
    struct stat dirStat;
    int i = 0;
    int fileCheck = 1;

    // Go through all the entries
    while ((aDir = readdir(currDir)) != NULL)
    {
        if (strncmp(PREFIX, aDir->d_name, strlen(PREFIX)) == 0 && strcmp(EXTENSION, strrchr(aDir->d_name,'.')) == 0)
        {
            fileCheck = 0;

            // Get meta-data for the current entry
            stat(aDir->d_name, &dirStat);  
            
            // Use the difftime function to get the time difference between the current value of lastModifTime and the st_mtime value of the directory entry
            if(i == 0 || dirStat.st_size < fileSize){
                fileSize = dirStat.st_size;
                memset(fileName, '\0', lengthFileName);
                strcpy(fileName, aDir->d_name);
                }
            i++;
        }
    }
    // Close the directory
    closedir(currDir);
    if (fileCheck == 1) printf("No files that begin with prefix 'movies_' and has a '.csv' extension exists");
    return fileCheck;
}

int getUserInputFile(char *fileName, int lengthFileName)
{
    // set up directory traverse variables and initialize currDir
    DIR *currDir = opendir(".");
    struct dirent *aDir;
    int fileSize;
    struct stat dirStat;
    int fileCheck = 1;
    char userEntry[256];

    // prompts user for desired file name
    printf("Enter the complete file name: ");
    scanf("%s", userEntry);

    // Go through all the entries and scan for match to userEntry
    while ((aDir = readdir(currDir)) != NULL)
    {
        if (strcmp(userEntry, aDir->d_name) == 0)
        {
            memset(fileName, '\0', lengthFileName);
            strcpy(fileName, aDir->d_name);
            fileCheck = 0;
        }
    }

    if (fileCheck == 1) {
        printf("The file %s was not found. Try again\n", userEntry);
    }

    // Close the directory
    closedir(currDir);
    return fileCheck;
}

void setDirectoryName(char *directoryName, char *onid)
{
    int upper = 99999;
    int lower = 0;
    
    strcpy(directoryName, onid);
    strcat(directoryName, ".movies.");

    srandom(time(0));
    int num = (random() % (upper - lower + 1)) + lower;
    char temp[6];
    sprintf(temp, "%d", num);

    strcat(directoryName, temp);
}
