/* struct for movie information */
struct movie;

/* Parse the current line which is space delimited and create a
*  movie struct with the data in this line
*/
struct movie *createMovie(char *currLine, int movieID);

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
* Print the linked list of movie for a given year
*/
void printMovieListByYear(int year, struct movie *list);

/*
* Print the linked list of movie for a given language
*/
void printMovieListByLanguage(char *language, struct movie *list);

void printHighestRatedMovieByYear(struct movie *list);

/*
* Print the instructions for what details the program can provide
*/
void printInstructions();

int getUserFeatureChoice();
