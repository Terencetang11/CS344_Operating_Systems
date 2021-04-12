/* Name : Terence Tang
*  Course : CS344 - Operating Systems
*  Date : Apr 12, 2021
*  Assignment #1: Movies
*  Description:  Header file for defining the movies library of functions used for this assignment.
*/

/* struct for movie information */
struct movie;

/* Parse the current line which is space delimited and create a
*  movie struct with the data in this line
*/
struct movie *createMovie(char *currLine);

/*
* Return a linked list of movies by parsing data from
* each line of the specified file.
*/
struct movie *processFile(char *filePath);

/*
* Print data for the given movie
*/
void printMovie(struct movie* aMovie);

/*
* Print the linked list of movie
*/
void printMovieList(struct movie *list);

/*
* Print the list of movies for a given year
*/
void printMovieListByYear(int year, struct movie *list);

/*
* Print the list of movies for a given language
*/
void printMovieListByLanguage(char *language, struct movie *list);

/*
* Print the highest rating movie for each year
*/
void printHighestRatedMovieByYear(struct movie *list);

/*
* Print the instructions for what details the program can provide
*/
void printInstructions();

/*
* Prompts user for input regarding which search feature they 
* would like to utilize on the given list of movies
*/
int getUserFeatureChoice();