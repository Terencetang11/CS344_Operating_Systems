#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>

int main(void)
{    
	char* newFilePath = "./movies_sample_1.csv";
	
	ssize_t nread, nwritten;
	char readBuffer[3200];
	
  // Open the file using the open system call
  // The flag O_RDWR means the file should be opened for reading and writing
  // The flag O_CREAT means that if the file doesn't exist, open should create it
  // The flag O_TRUNC means that if the file already exits, open should truncate it.
	FILE *file_descriptor = fopen(newFilePath, "r");
	// if (file_descriptor == -1){
	// 	printf("open() failed on \"%s\"\n", newFilePath);
	// 	perror("In main()");
	// 	exit(1);
	// }
	
//   // Write to the file using the write system call
// 	nwritten = write(file_descriptor, giveEm, strlen(giveEm) * sizeof(char));
  // Clear out the array before using it
	memset(readBuffer, '\0', sizeof(readBuffer)); 

  // Reset the file pointer to the beginning of the file
	// lseek(file_descriptor, 0, SEEK_SET); 

//   // Read from the file using the read system call
// 	nread = read(file_descriptor, readBuffer, sizeof(readBuffer));
// 	printf("nread = %zd, nwritten = %zd\n", nread, nwritten);
	
// 	printf("File contents:\n%s", readBuffer);

    char *currLine = NULL;
    size_t len = 0;

    nread = getline(&currLine, &len, file_descriptor);
    printf("%s\n", currLine);

    nread = getline(&currLine, &len, file_descriptor);
    printf("%s\n", currLine);

    // variables for testing
    char *title;
    int year;
    
    // For use with strtok_r
    char *saveptr;

    // The first token is the ONID
    char *token = strtok_r(currLine, ",", &saveptr);
    title = calloc(strlen(token) + 1, sizeof(char));
    strcpy(title, token);
    printf("%s\n", title);
	
    // The second token is the ONID
    token = strtok_r(NULL, ",", &saveptr);
    year = atoi(token);
    printf("%d\n", year);

	return 0;
}