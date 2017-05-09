/*
*  < AUTHOR >
*/

// module includes
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <sys/wait.h>

// MAX COMMAND LENGTH
#define MAX_CMD_LENGTH 255
// NEW LINE CHAR
#define NEW_LINE_CHAR "\n"

// String Array Data Structure
struct StringArray {
  char** store;
  int size;
};

// function to read command from terminal
char * readCommand(int);

// function to split str by delim and return it as array
struct StringArray splitStrByDelim(char *, const char []);

// function to create a new process and execute a command passed to it
void forkAndExecuteCommand();

// function to print a char array with given length
void printArray(char **, int);
