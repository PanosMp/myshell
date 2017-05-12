/*
*  Panagiotis Mparmpagiannis (AM: 3110124)
*  Tsifrikas Georgios (AM: 3110205)
*/

// include env files

#include <stdio.h>
#include <errno.h>
#include <ctype.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

// MAX COMMAND LENGTH
#define MAX_CMD_LENGTH 255
// NEW LINE CHAR
#define NEW_LINE_CHAR "\n"
// SPACE DELIM
#define SPACE_DELIM " "
// Redirect input
#define RE_IN "<"
// Redirect output
#define RE_OUT ">"
// Redirect + append output
#define RA_OUT ">>"
// pipe delim
#define PIPE_DELIM "|"

// String Array Data Structure
struct StringArray {
  char** store;
  int size;
};

// Command Data Structure
struct Command{
  char* cmd;
  int param_size;
  char** parameters;
  int in;
  int out;
};

// Pipeline Struct
struct PipelineStore{
  struct Command *store;
  int size;
};

// fn to prin pipeline struct
void printPipeline(struct PipelineStore);

// function to read command from terminal
char * readCommand(int);

// function to split str by delim and return it as array
struct StringArray splitStrByDelim(char *, const char []);

// function to create a new process and execute a command passed to it
void forkAndExecuteCommand(struct Command, int);

// fork supporting redirections
void forkAndExecuteCommandWithRedirect(struct Command, int);

// remove leading/trailing white spaces from a str
char *trimwhitespace(char *str);

// transform to StringArray to Command
struct Command seperateCmdParams(struct StringArray);

// function to print a char array with given length
void printArray(char **, int);

// filter command for descriptors
struct Command filterCommandFd(struct Command);

// print command struct
void printCommandStruct(struct Command);

// fork handler
int fork_pipes (int , struct PipelineStore );

// int spawn_proc (int, int, struct Pipeline *);
int spawn_proc (int , int , struct Command );

// transform command to pipeline struct
struct PipelineStore transformPipes(struct StringArray);
