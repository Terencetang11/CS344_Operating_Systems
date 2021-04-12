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
*   Process the file provided as an argument to the program to
*   create a linked list of student structs and print out the list.
*   Compile the program as follows:
*       gcc --std=gnu99 -o students main.c
*/

int main(int argc, char *argv[])
{
    // checks to see if a filepath was provided for the input movie data
    if (argc < 2)
    {
        printf("You must provide the name of the file to process\n");
        printf("Example usage: ./movies_sample_1.csv\n");
        return EXIT_FAILURE;
    }

    // processes given filepath and parses data into linked list - returns pointer to head node
    struct movie *list = processFile(argv[1]);
    
    // continuously prompts user for desired interactivity with movie linked list
    int choice = getUserFeatureChoice();
    while (choice != 4){

        // case for if an invalid integer choice was provided
        if (choice < 1 || choice > 4)
        {
            printf("You entered an incorrect choice. Try again.\n");
        }

        // case for if choice 1 was selected, prompts for a year and prints movies released in that year
        else if (choice == 1)
        {
            int year;
            printf("Enter the year for which you want to see movies: ");
            scanf("%d", &year);
            printMovieListByYear(year, list);
        }

        // case for if choice 2 was selected, prints highest rated movie for each year
        else if (choice == 2)
        {
            printHighestRatedMovieByYear(list);
        }

        // case for if choice 3 was selected, prompts for a language and prints movies released in that lang
        else if (choice == 3)
        {
            char language[21];
            printf("Enter the language for which you want to see movies: ");
            scanf("%20s", language);
            printMovieListByLanguage(language, list);
        }
        choice = getUserFeatureChoice();    // continues to ask if user would like to do something else
    }

    return EXIT_SUCCESS;
}
