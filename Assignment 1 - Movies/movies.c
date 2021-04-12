// If you are not compiling with the gcc option --std=gnu99, then
// uncomment the following line or you might get a compiler warning
//#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "movies.h"


/* struct for movie information */
struct movie
{
    int movieID;
    char *title;
    int year;
    char *language;
    char language2[5][21];
    float ratingValue;
    struct movie *next;
};

/* 
* Parse the current line which is space delimited and create a
* student struct with the data in this line
*/
struct movie *createMovie(char *currLine, int movieID)
{
    struct movie *currMovie = malloc(sizeof(struct movie));
    currMovie->movieID = movieID;

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

    // The next token is the movie language
    token = strtok_r(NULL, ",", &saveptr);
    currMovie->language = calloc(strlen(token) + 1, sizeof(char));
    strcpy(currMovie->language, token);

    // remove first and last [] chars from list of languages
    token++;
    token[strlen(token)-1] = 0;
    int index = 0;

    // The first language in the list of languages
    char *tokenLang = strtok_r(token, ";", &saveptrLang);
    strcpy(currMovie->language2[index] , tokenLang);
    tokenLang = strtok_r(NULL, ";", &saveptrLang);

    // Parse through the list of languages and add to language array in movie struct
    while (tokenLang != NULL)
    {
        index++;
        strcpy(currMovie->language2[index] , tokenLang);
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
    int movieID = 0;

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
        struct movie *newNode = createMovie(currLine, movieID);

        // Is this the first node in the linked list?
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
    free(currLine);
    fclose(movieFile);

    printf("Processed file %s and parsed data for %d movies\n", filePath, tail->movieID);

    return head;
}

/*
* Print data for the given movie
*/
void printMovie(struct movie* aMovie)
{
    printf("%s, %d %s, %f\n", aMovie->title,
               aMovie->year,
               aMovie->language,
               aMovie->ratingValue);
    
    for (int i = 0; i < 5; i++)
    {
        printf("language #%d: %s\n", i+1, aMovie->language2[i]);
    }
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
    int count = 0;
    while (list != NULL)
    {
        if (list->year == year)
        {
            printf("%s\n", list->title);
            count++;
        }
        list = list->next;
    }
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
    int count = 0;
    while (list != NULL)
    {
        for (int i = 0; i < 5; i++){
            if (strcmp(list->language2[i],language) == 0)
            {
                printf("%d %s\n", list->year, list->title);
                count++;
            }
        }
        list = list->next;
    }
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
    int yearsChecked[2022-1899] = {0};
    int count = 0;
    while (list != NULL)
    {
        struct movie *temp = list;
        struct movie *top = list;
        yearsChecked[count] = list->year;
        
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