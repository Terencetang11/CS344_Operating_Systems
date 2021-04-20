/* Name : Terence Tang
*  Course : CS344 - Operating Systems
*  Date : Apr 12, 2021
*  Assignment #1: Movies
*  Description:  Header file for defining the movie_processing and file_processing library of functions 
*                used for this assignment.
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
* Cleans up memory used for creation of movie link list
* Call once you've finished with list
*/
void free_movie_memory(struct movie *list);

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
void printInstructions(int *programChoice);

/*
* Print the instructions for what type of file processing the program can provide
*/
void printFileProcessingInstructions(int *fileChoice);

/*
* Parses user input on which type of file they would like to process
*/
int getFileName(int *choice, char *fileName, int lengthFileName);

/*
* Scans current active directory for the largest file (by byte size) given 
* specific criteria for file prefix and extension suffixes
*/
int getLargestMovieCSV(char *fileName, int lengthFileName);

/*
* Scans current active directory for the smallest file (by byte size) given 
* specific criteria for file prefix and extension suffixes
*/
int getSmallestMovieCSV(char *fileName, int lengthFileName);

/*
* Prompts user for a file name and checks if the file exists in the current active directory 
*/
int getUserInputFile(char *fileName, int lengthFileName);

/*
* Generates a new directory name with a randomly generated int
*/
void setDirectoryName(char *directoryName, char *onid);

/*
* Takes a given directory path and movie list and writes a new file for each year a 
* movie was released from the movie list.  Then it prints all movies in the movie list
* to their respective release year files.
* File naming format = YYYY.txt
*/
void writeMoviesToDirectory(struct movie *list, char *directoryName);




