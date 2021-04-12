// If you are not compiling with the gcc option --std=gnu99, then
// uncomment the following line or you might get a compiler warning
//#define _GNU_SOURCE

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
    if (argc < 2)
    {
        printf("You must provide the name of the file to process\n");
        printf("Example usage: ./movies_sample_1.csv\n");
        return EXIT_FAILURE;
    }
    struct movie *list = processFile(argv[1]);
        
    int choice = getUserFeatureChoice();
    while (choice != 4){
        if (choice < 1 || choice > 4)
        {
            printf("You entered an incorrect choice. Try again.\n");
        }
        else if (choice == 1)
        {
            int year;
            printf("Enter the year for which you want to see movies: ");
            scanf("%d", &year);
            printMovieListByYear(year, list);
        }
        else if (choice == 2)
        {
            printHighestRatedMovieByYear(list);
        }
        else if (choice == 3)
        {
            char language[21];
            printf("Enter the language for which you want to see movies: ");
            scanf("%20s", language);
            printMovieListByLanguage(language, list);
        }
        choice = getUserFeatureChoice();
    }


    return EXIT_SUCCESS;
}
