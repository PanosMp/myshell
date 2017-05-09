/*
*  < AUTHOR >
*/

// include custom files
#include "mysh-common.h"

// include env files
#include <string.h>
#include <stdlib.h>

// command struct
struct Command {
   char* cmd;
   char** cmdArguments;
};

// function to read command from the stdin
char * readCommand(int shellID) {

  // var to store the command
  char *text = calloc(1,1);
  // buffer to read from std
  char buffer[MAX_CMD_LENGTH];

  // prompt user
  printf("mysh%d>", shellID);

  // read from stdin and store the data in the buffer
  if(fgets(buffer, MAX_CMD_LENGTH - 1, stdin) == NULL){

    printf("\nExiting shell... \n");
    return NULL;
  }

  // allocate memory
  text = realloc( text, strlen(text)+1+strlen(buffer) );

  // if allocation failed
  if( !text ) {
    fprintf(stderr, "ERROR: Error allocating memory. \n");
    fprintf(stderr, "ERROR: Prepare to shut down..\n");
    fprintf(stderr, "ERROR: Terminating shell...\n");
    exit(EXIT_FAILURE);
  }

  if(buffer == NULL){
    return NULL;
  }

  // append buffer to text
  strcat( text, buffer ); /* note a '\n' is appended here everytime */

  // return text;
  return text;
}

// split Command
struct StringArray splitStrByDelim(char * str, const char delim[]){

   // struct to keep the collection
   struct StringArray strArray;
   // var to hold the tokens
   char *token;
   // the array to store the tokens
   char **tokenStore = malloc(1 * sizeof(char));
   // the size of the array
   int storeSize = 0;

   // take the first token
   token = strtok(str, delim);

   /* walk through other tokens */
   while( token != NULL )
   {
      if( (token != NULL) && (strcmp(token, NEW_LINE_CHAR) != 0)){
        tokenStore = realloc(tokenStore, ((storeSize + 1) * sizeof(char*)));
        tokenStore[storeSize] = malloc((strlen(token)+1) * sizeof(char*));
        tokenStore[storeSize] = token;
        storeSize++;
      }
      token = strtok(NULL, delim);
   }

   // assing the tokenStore to the value
   strArray.store = tokenStore;
   strArray.size = storeSize;

   return strArray;
  //  free(tokenStore);
  //  return test;
}

void printArray(char ** array, int len){
  printf("------------------ \n");
  printf("Array Len: %d \n", len);
  printf("------------------ \n");

  for(int i = 0; i < len; i++){
    printf("\nitem at %d, with value: %s",i,array[i]);
  }
}

// void forkAndExecuteCommand(){
//
//   struct Command cmd;
//   strcpy( cmd.cmd, "ls");
//   strcpy( cmd.cmdArguments[0], "ls");
//   strcpy( cmd.cmdArguments[1], "-la");
//   strcpy( cmd.cmd[2], NULL);
//
//   // process id and wait process id
//   pid_t pid, waitPid;
//
//   // the process status
//   int status;
//
//   // fork current proccess
// 	pid = fork();
//
//   // if fork fails
// 	if (pid < 0) {
// 		fprintf(stderr, "ERROR: Fork failed.\n");
// 		exit(EXIT_FAILURE);
// 	}
//
// 	// child process
// 	if (pid == 0) {
//
//     execvp(cmd.cmd, cmd.cmdArguments);
// 		perror("ERROR: Child should never arrive here.\n");
// 	}
// 	// parent process
// 	else {
//     // wait for child to finish
//     waitPid = wait(&status);
// 		if (waitPid == -1) {
// 			fprintf(stderr, "ERROR: Waitpid failed.\n");
// 			exit(EXIT_FAILURE);
// 		}
//   }
// }
