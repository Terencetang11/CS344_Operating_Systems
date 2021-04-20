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
#include <time.h> // not needed
#include <unistd.h>
#include "file_and_directory_processing.h"

#define PREFIX "movies_"
#define EXTENSION ".csv"

/*
* Parses user input on which type of file they would like to process
*/
int getFileName(int *fileChoice, char *fileName, int lengthFileName)
{
    int fileCheck = 1;
    // parses user input to specific functionality, returns int value for if file found
    // (fileCheck = 0 means found, 1 means err not found)
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

/*
* Scans current active directory for the largest file (by byte size) given 
* specific criteria for file prefix and extension suffixes
*/
int getLargestMovieCSV(char *fileName, int lengthFileName)
{
    // set up directory traverse variables and initialize currDir
    DIR *currDir = opendir(".");
    struct dirent *aDir;
    int fileSize;
    struct stat dirStat;
    int i = 0;
    int fileCheck = 1;

    // Go through all the entries in current directory
    while ((aDir = readdir(currDir)) != NULL)
    {
        // check if current entry has a valid prefix ("movies_") and file extension (".csv") 
        if (strncmp(PREFIX, aDir->d_name, strlen(PREFIX)) == 0 && strcmp(EXTENSION, strrchr(aDir->d_name,'.')) == 0)
        {
            // if file found, updates return value for validation
            fileCheck = 0;

            // Get meta-data for the current entry
            stat(aDir->d_name, &dirStat);  
            
            // Use the entry metadata to determine size, checks if current file size is larger than 
            // the currently tracked largest found file thus far.  If not, updates file name for largest found.
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

/*
* Scans current active directory for the smallest file (by byte size) given 
* specific criteria for file prefix and extension suffixes
*/
int getSmallestMovieCSV(char *fileName, int lengthFileName)
{
    // set up directory traverse variables and initialize currDir
    DIR *currDir = opendir(".");
    struct dirent *aDir;
    int fileSize;
    struct stat dirStat;
    int i = 0;
    int fileCheck = 1;

    // Go through all the entries in current directory
    while ((aDir = readdir(currDir)) != NULL)
    {
        // check if current entry has a valid prefix ("movies_") and file extension (".csv") 
        if (strncmp(PREFIX, aDir->d_name, strlen(PREFIX)) == 0 && strcmp(EXTENSION, strrchr(aDir->d_name,'.')) == 0)
        {
            // if file found, updates return value for validation
            fileCheck = 0;

            // Get meta-data for the current entry
            stat(aDir->d_name, &dirStat);  
            
            // Use the entry metadata to determine size, checks if current file size is smaller than 
            // the currently tracked smallest found file thus far.  If not, updates file name for smallest found.            
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

/*
* Prompts user for a file name and checks if the file exists in the current active directory 
*/
int getUserInputFile(char *fileName, int lengthFileName)
{
    // set up directory traverse variables and initialize currDir
    DIR *currDir = opendir(".");
    struct dirent *aDir;
    int fileSize;
    struct stat dirStat;
    int fileCheck = 1;
    char userEntry[256];

    // prompts user for desired file name and stores to userEntry
    printf("Enter the complete file name: ");
    scanf("%s", userEntry);

    // Go through all the entries and scan for match to userEntry
    while ((aDir = readdir(currDir)) != NULL)
    {
        // if match found, stores file to fileName and updates fileCheck value
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

/*
* Generates a new directory name with a randomly generated int
*/
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
