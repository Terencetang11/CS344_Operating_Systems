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
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include "file_and_directory_processing.h"


/* struct for movie information */
struct movie
{
    char *title;
    int year;
    char language[5][21];   // array set to store up to max 5 language of len 20 chars
    float ratingValue;
    struct movie *next;
};

/* 
* Parse the current line which is space delimited and create a
* student struct with the data in this line
*/
struct movie *createMovie(char *currLine)
{
    struct movie *currMovie = malloc(sizeof(struct movie));

    // For use with strtok_r
    char *saveptr;
    char *saveptrLang;

    // The first token is the movie title
    char *token = strtok_r(currLine, ",", &saveptr);
    currMovie->title = calloc(strlen(token) + 1, sizeof(char));
    strcpy(currMovie->title, token);

    // The next token is the release year
    token = strtok_r(NULL, ",", &saveptr);
    currMovie->year = atoi(token);

    // The next token is the list of movie languages
    token = strtok_r(NULL, ",", &saveptr);

    // remove first and last [] chars from list of languages
    token++;
    token[strlen(token)-1] = 0;
    int index = 0;

    // The first language in the list of languages
    char *tokenLang = strtok_r(token, ";", &saveptrLang);
    strcpy(currMovie->language[index] , tokenLang);
    tokenLang = strtok_r(NULL, ";", &saveptrLang);

    // Parse through the list of languages and add to language array in movie struct
    while (tokenLang != NULL)
    {
        index++;
        strcpy(currMovie->language[index] , tokenLang);
        tokenLang = strtok_r(NULL, ";", &saveptrLang);
    }

    // The last token is the rating value
    token = strtok_r(NULL, "\n", &saveptr);
    currMovie->ratingValue = atof(token);

    // Set the next node to NULL in the newly created movie entry
    currMovie->next = NULL;

    return currMovie;
}

/*
* Return a linked list of movies by parsing data from
* each line of the specified file.
*/
struct movie *processFile(char *filePath)
{
    // Open the specified file for reading only
    FILE *movieFile = fopen(filePath, "r");

    char *currLine = NULL;
    size_t len = 0;
    ssize_t nread;
    char *token;
    int movieID = 0;    // movieID used to track # of movies parsed

    // The head of the linked list
    struct movie *head = NULL;
    // The tail of the linked list
    struct movie *tail = NULL;

    // get rid of the header line in the movie file
    nread = getline(&currLine, &len, movieFile);

    // Read the file line by line
    while ((nread = getline(&currLine, &len, movieFile)) != -1)
    {
        movieID++;

        // Get a new movie node corresponding to the current line
        struct movie *newNode = createMovie(currLine);

        // Checks if this is the first node of linked list and points head / tail
        if (head == NULL)
        {
            // This is the first node in the linked link
            // Set the head and the tail to this node
            head = newNode;
            tail = newNode;
        }
        else
        {
            // This is not the first node.
            // Add this node to the list and advance the tail
            tail->next = newNode;
            tail = newNode;
        }
    }

    // garbage collection - frees unneeded memory and closes filepath file
    free(currLine);
    fclose(movieFile);

    // printf("Processed file %s and parsed data for %d movies\n", filePath, movieID);
    return head;
}

/*
* Cleans up memory used for creation of movie link list
* Call once you've finished with list
*/
void free_movie_memory(struct movie *list)
{
    struct movie *current = list;
    struct movie *next;
    while (current != NULL)
    {
        next = current->next;
        free(current->title);
        free(current);
        current = next;
    }
}

/*
* Print data for the given movie
*/
void printMovie(struct movie* aMovie)
{
    printf("%s, %d, %f\n", aMovie->title,
               aMovie->year,
               aMovie->ratingValue);
    
    // cycles through array of published languages and prints them
    // for (int i = 0; i < 5; i++)
    // {
    //     printf("language #%d: %s\n", i+1, aMovie->language[i]);
    // }
}

/*
* Print the linked list of movie
*/
void printMovieList(struct movie *list)
{
    while (list != NULL)
    {
        printMovie(list);
        list = list->next;
    }
}

/*
* Print the linked list of movie for a given year
*/
void printMovieListByYear(int year, struct movie *list)
{
    // checks if there is a movie in given year
    int count = 0;

    // scans linked list and if movie year equals given year, prints movie details
    while (list != NULL)
    {
        if (list->year == year)
        {
            printf("%s\n", list->title);
            count++;
        }
        list = list->next;
    }

    // prints statement if no movie data exists for given year
    if (count == 0)
    {
        printf("No data about movies released in the year %d\n", year);
    }
}

/*
* Print the linked list of movie for a given language
*/
void printMovieListByLanguage(char *language, struct movie *list)
{
    // checks if there is a movie in given language
    int count = 0;

    // scans linked list and if movie year equals given language, prints movie details
    while (list != NULL)
    {
        // scans through array of languages for given movie
        for (int i = 0; i < 5; i++){
            if (strcmp(list->language[i],language) == 0)
            {
                printf("%d %s\n", list->year, list->title);
                count++;
            }
        }
        list = list->next;
    }

    // prints statement if no movie data exists for given language
    if (count == 0)
    {
        printf("No data about movies released in %s\n", language);
    }
}

/*
* Prints the highest rated movie for each year
*/
void printHighestRatedMovieByYear(struct movie *list)
{
    // initializes a datastructure to track what years we've already found movies for
    int yearsChecked[2022-1899] = {0};
    int count = 0;
    
    // checks each node of the linked list to see if we've found a top movie for that year
    while (list != NULL)
    {
        struct movie *temp = list;
        struct movie *top = list;
        yearsChecked[count] = list->year;
        
        // for new 'year', checks all other nodes to see if theres a higher rated movie
        while (temp != NULL)
        {
            if (temp->year == top->year){
                if (temp->ratingValue > top->ratingValue)
                    top = temp;
            }
            temp = temp->next;
        }
        printf("%d, %.1f, %s\n", top->year, top->ratingValue, top->title);
        count++;

        // when movie to next link, checks to see if that node's year is already found
        for (int i = 0; i < count; i++)
        {
            if (list != NULL && yearsChecked[i] == list->year)
            {
                list = list->next;
                i = -1;
            }
        }

    }
}


/*
* Takes a given directory path and movie list and writes a new file for each year a 
* movie was released from the movie list.  Then it prints all movies in the movie list
* to their respective release year files.
* File naming format = YYYY.txt
*/
void writeMoviesToDirectory(struct movie *list, char *directoryPath)
{
    // initiates variables for storing file name / path and a buffer of data to write to files
    int file_descriptor;
    char strYear[5];
    int lengthDirectoryPath = strlen(directoryPath);
    char file_name[11+lengthDirectoryPath];
    char *buffer;

    // stores directory path string locally (fixes odd pointer issues w/ strcat)
    char *dirPath;
    dirPath = calloc(strlen(directoryPath) + 1, sizeof(char));
    strcpy(dirPath, directoryPath);

    // read file line item
    while (list != NULL)
    {
        // clear previous file_name string
        memset(file_name, '\0', strlen(file_name));

        // create new file_path based on input movie year
        strcpy(file_name,"./");
        strcat(file_name,dirPath);
        strcat(file_name, "/");
        sprintf(strYear, "%d", list->year);
        strcat(file_name,strYear);
        strcat(file_name, ".txt");

        // open / create corresponding year file with appropriate flags and permissions
        file_descriptor = open(file_name, O_WRONLY | O_CREAT | O_APPEND, 0640);

        // get movie title and concatenate with new line char, saves to buffer
        buffer = calloc(strlen(list->title) + 3, sizeof(char));
        strcpy(buffer, list->title);
        strcat(buffer, "\n");

        // writes buffer to file
        write(file_descriptor, buffer, strlen(buffer));
        free(buffer);

        // moves to next movie
        list = list->next;
    }
    free(dirPath);
}