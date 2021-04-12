// If you are not compiling with the gcc option --std=gnu99, then
// uncomment the following line or you might get a compiler warning
//#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "movies.h"

void printInstructions()
{
    printf("\n");
    printf("1. Show movies released in the specified year\n");
    printf("2. Show highest rated movie for each year\n");
    printf("3. Show the title and year of release of all movies in a specific language\n");
    printf("4. Exit from the program\n");
    printf("\n");
}

int getUserFeatureChoice()
{
    printInstructions();
    int choice;
    printf("Enter a choice from 1 to 4: ");
    scanf("%d", &choice);
    return choice;
}