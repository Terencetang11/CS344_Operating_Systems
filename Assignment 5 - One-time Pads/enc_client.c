/* 
*  Name : Terence Tang
*  Course : CS344 - Operating Systems
*  Date : May 30 2021
*  Assignment #5: One-Time Pads - Encryption Client
*  Description:  Client which takes user provided plaintext and key files and sends  encryption requests to an 
*                localhost encryption service via a provided port number.  The data transfer is made via socket 
*                communications.  The client checks plaintext and key inputs for valid length and input characters
*                before sending a request to the encryption service.  It then checks that it has connected to the 
*                right service and sends all relevant data.  It then waits to receive a ciphertext response which
*                it prints to stdout (which can be redirected).
*
*/

#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>  // ssize_t
#include <sys/socket.h> // send(),recv()
#include <netdb.h>      // gethostbyname()


// Declare Global Resources 
static const char *HOSTNAME = "localhost";                              // hostname used in creating socket connection requests
static const int MAX_MSG_SIZE = 100000;                                 // maximum size of data to be sent
static const int MAX_TRANSMISSION_SIZE = 1024;                          // maximum size of data for transmission to server
static const char validChars[27] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ ";       // set of all valid input characters A-Z and SPACE

// Error function used for reporting issues with errno
void error(const char *msg)
{ 
  perror(msg); 
  exit(0); 
} 

// Set up the address struct
void setupAddressStruct(struct sockaddr_in* address, int portNumber)
{
    // Clear out the address struct
    memset((char*) address, '\0', sizeof(*address)); 

    // The address should be network capable
    address->sin_family = AF_INET;
    // Store the port number
    address->sin_port = htons(portNumber);

    // Get the DNS entry for this host name
    struct hostent* hostInfo = gethostbyname(HOSTNAME); 
    if (hostInfo == NULL) { 
        fprintf(stderr, "CLIENT: ERROR, no such host found for %s\n", HOSTNAME); 
        exit(2); 
    }
    // Copy the first IP address from the DNS entry to sin_addr.s_addr
    memcpy((char*) &address->sin_addr.s_addr, 
            hostInfo->h_addr_list[0],
            hostInfo->h_length);
}

// Send data via provided socket
int sendData(int socketFD, char *msg)
{
    int charsWritten = send(socketFD, msg, strlen(msg), 0);                 // Sends msg data to a socket File Descriptor
    if (charsWritten < 0)                                                   // Error check sent data
    {                                          
        error("SERVER: ERROR reading from socket");
    }
    if (charsWritten < strlen(msg))
    {
        fprintf(stderr,"SERVER: WARNING: Not all data written to socket!\n"); 
    }
    return charsWritten;
}

// Read data via provided socket
int readData(int socketFD, char *buffer, int bufferLen)
{
    memset(buffer, '\0', bufferLen);                                        // Clear out buffers and charsRead for next recv
    int charsRead = 0;
    while (charsRead == 0)                                                  // Wait for next available recv data
    {
        charsRead = recv(socketFD, buffer, bufferLen - 1, 0);               // Read the client's message from the socket
    }
    if (charsRead < 0)                                                      // Error check received data
    {                                          
        error("SERVER: ERROR writing to socket");
    }
    if (charsRead < strlen(buffer))
    {
        fprintf(stderr,"SERVER: WARNING: Not all read from socket!\n"); 
    }
    return charsRead;
}

// Check if input file contains only valid chars via validChar mapping
void checkFileForValidChars(char *input, char *fileName)
{
    // replace new line chars with null terminating chars
    input[strcspn(input, "\n")] = '\0'; 

    // iterate through each char in file
    for (int j = 0; j < strlen(input); j++) {
        // compare char to each valid char from validChar mapping
        for (int i = 0; i < sizeof(validChars) + 1; i++)
        {   
            // if no match found throw invalid input error and exit
            if (i == 27)
            {
                fprintf(stderr, "Error: %s contains invalid characters.\n", fileName);
				exit(1);
            }
            // if match found, move onto next char in file
            if (input[j] == validChars[i])
            {
                break;
            }
        }
    }
}

int main(int argc, char *argv[])
{
    int socketFD, portNumber, charsWritten, charsRead, plaintextLen, keyLen;
    struct sockaddr_in serverAddress;
    char buffer[MAX_TRANSMISSION_SIZE];
    char plaintext[MAX_MSG_SIZE];
    char key[MAX_MSG_SIZE];
    char ciphertext[MAX_MSG_SIZE];
    char filePath[256];


    /*-- Check usage & args --*/
    if (argc < 4) 
    { 
        fprintf(stderr,"USAGE: \'%s\' plaintext key port\n", argv[0]); 
        exit(0); 
    }

    /*-- Check Plaintext and Key inputs --*/
    // Clears all string storage for input
    memset(filePath, '\0', sizeof(filePath));   
    memset(plaintext, '\0', sizeof(plaintext));                           
    memset(key, '\0', sizeof(key));                           

    // Open specified plaintext file text for read only
    strcpy(filePath, "./");                                                 // generate filepath
    strcat(filePath, argv[1]); 
    FILE *plaintextFile = fopen(filePath, "r");
    if (plaintextFile == NULL)                                              // error handling for invalid file
    {
        fprintf(stderr,"Invalid File: specified plaintext file \'%s\' not found\n", argv[1]); 
        exit(1);
    }

    // Check if Plaintext file has valid chars and store file size
    fgets(plaintext, sizeof(plaintext) - 1, plaintextFile);                 // stores file contents to string
    fseek(plaintextFile, 0, SEEK_SET);                                      // reset pointer for next use
    checkFileForValidChars(plaintext, argv[1]);
    plaintextLen = strlen(plaintext);                                       // store plaintext len
// printf("plaintext input size: %d\n", plaintextLen);
// printf("plaintext text: %s\n", plaintext);

    // Open specified key file text for read only
    memset(filePath, '\0', sizeof(filePath));                               // clear filepath variable                
    strcpy(filePath, "./");                                                 // generate filepath
    strcat(filePath, argv[2]); 
    FILE *keyFile = fopen(filePath, "r");
    if (keyFile == NULL)                                                    // error handling for invalid file
    {
        fprintf(stderr,"Invalid File: specified key file %s not found\n", argv[2]);
        exit(1);
    }

    // Check if key file has valid chars and store file size
    fgets(key, sizeof(key) - 1, keyFile);                                   // stores file contents to string
    fseek(keyFile, 0, SEEK_SET);                                            // reset pointer for next use
    checkFileForValidChars(key, argv[2]);
    keyLen = strlen(key);                                                   // store key len
// printf("key input size: %d\n", keyLen);
// printf("key text: %s\n", key);

    // check if plaintext is greater than key size, throw error and exit if true
    if(keyLen < plaintextLen){ 
        fprintf(stderr,"Error: key \'%s\' is too short\n", argv[2]);
        exit(1);
	}

    /*-- Create and Validate Encryption Socket Connection --*/
    // Create a socket
    socketFD = socket(AF_INET, SOCK_STREAM, 0); 
    if (socketFD < 0)
    {
        error("CLIENT: ERROR opening socket");
		fprintf(stderr, "Error: could not contact enc_server on port %d\n", atoi(argv[3]));
        exit(2);
    }

    // Set up the server address struct
    setupAddressStruct(&serverAddress, atoi(argv[3]));

    // Connect to server
    if (connect(socketFD, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) < 0)
    {
        error("CLIENT: ERROR connecting to server");
		fprintf(stderr, "Error: could not contact enc_server on port %d\n", atoi(argv[3]));
        exit(2);
    }

    /*-- Validate Socket Connection Is For Right Encryption Service --*/
    char* checkMsg = "enc_server";                                          // Send request type to server
    sendData(socketFD, checkMsg);                                                               
    charsRead = readData(socketFD, buffer, sizeof(buffer));                 // Receive server response - accepted or denied
// printf("server response: %s\n", buffer);
    if(strcmp(buffer, "denied") == 0){                                      // Check if server affirms correct connection type
		fprintf(stderr, "Error: enc_client cannot use dec_server on port %d\n", atoi(argv[3]));
		exit(2);    // if invalid, else exits
	}

    /*-- Send Plaintext Data to Encryption Server --*/
    // Send data length
    memset(buffer, '\0', sizeof(buffer));                                   // Clear out buffers and charsread for next send
	sprintf(buffer, "%d", plaintextLen);
// printf("buffer value: %s\n", buffer);
    sendData(socketFD, buffer);                                             // Send plaintext length to server
    charsRead = readData(socketFD, buffer, sizeof(buffer));                 // Receive server response - continue msg
// printf("server response: %s\n", buffer);

    // Send plaintext data to enc_server
    charsWritten = 0;
    // continues to write plaintext data to client until plaintext length reached
    while (charsWritten < plaintextLen)
    {
        memset(buffer, '\0', sizeof(buffer));                               // Clear out buffer for next line
        fgets(buffer, sizeof(buffer) - 1, plaintextFile);                   // Get buffer sized chunk from plaintext
        buffer[strcspn(buffer, "\n")] = '\0';                               // Remove the trailing \n that fgets adds
        charsWritten += sendData(socketFD, buffer);
    }
    charsRead = readData(socketFD, buffer, sizeof(buffer));                 // Receive server response - plaintext received msg
    if (strcmp(buffer, "Plaintext Received") != 0)                          // Confirm server received plaintext
    {
        fprintf(stderr, "ERROR: Server did not receive plaintext data\n"); 
        exit(2); 
    }
// printf("server plaintext response: %s\n", buffer);

    /*-- Send Key Data to Encryption Server --*/
    // Send key data to enc_server
    charsWritten = 0;
    // continues to write key data to client until plaintext length reached
    while (charsWritten < plaintextLen)
    {
        memset(buffer, '\0', sizeof(buffer));                               // Clear out buffer for next line
        fgets(buffer, sizeof(buffer) - 1, keyFile);                         // Get buffer sized chunk from plaintext
        buffer[strcspn(buffer, "\n")] = '\0';                               // Remove the trailing \n that fgets adds
        charsWritten += sendData(socketFD, buffer);
    }
    charsRead = readData(socketFD, buffer, sizeof(buffer));                 // Receive server response - key received msg
    if (strcmp(buffer, "Key Received") != 0)                                // Confirm server received key
    {
        fprintf(stderr, "ERROR: Server did not receive key data\n"); 
        exit(2); 
    }
    charsWritten += sendData(socketFD, "Waiting for ciphertext..");

// printf("server key response: %s\n", buffer);
    

    /*-- Receive Ciphertext Data from Server --*/
    memset(ciphertext, '\0', sizeof(ciphertext));
    charsRead = 0;
    // printf("cipher text: %s, chars read: %d\n", ciphertext, charsRead);
    // continues to read cipher data from server until expected data length
    while (charsRead < plaintextLen)
    {
        // printf("in while loop\n");
        charsRead += readData(socketFD, buffer, sizeof(buffer));
        // printf("chars read: %d\n", charsRead);
        strcat(ciphertext, buffer);                                         // stores cipher text to ciphertext str
        // printf("buffer: %s\n cipher: %s\n", buffer, ciphertext);
    }
    printf("%s\n", ciphertext);                                             // prints ciphertext with added newline char

	close(socketFD); // Close the socket
	return 0;
}