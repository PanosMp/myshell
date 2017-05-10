/*
*  < AUTHOR >
*/

// include custom files
#include "mysh-common.h"

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
}

// Note: This function returns a pointer to a substring of the original string.
// If the given string was allocated dynamically, the caller must not overwrite
// that pointer with the returned value, since the original pointer must be
// deallocated using the same allocator with which it was allocated.  The return
// value must NOT be deallocated using free() etc.
char *trimwhitespace(char *str){

  char *end;

  // Trim leading space
  while(isspace((unsigned char)*str)) str++;

  if(*str == 0)  // All spaces?
    return str;

  // Trim trailing space
  end = str + strlen(str) - 1;
  while(end > str && isspace((unsigned char)*end)) end--;

  // Write new null terminator
  *(end+1) = 0;

  return str;
}

/*
  Function to split the array {cmd | <parameters>}
*/
struct Command seperateCmdParams(struct StringArray array) {
  // command struct
  struct Command t;
  // assign command to struct
  t.cmd = array.store[0];
  // assign the remaining values of the collection as parameters
  t.parameters = array.store;
  // the size of the parameters is the original size of the array
  t.param_size = array.size;

  // debug
  printf("Command: %s\n", t.cmd);
  printArray(t.parameters, t.param_size);
  return t;
}

/*
  Function to print a char array
*/
void printArray(char ** array, int len){
  printf("------------------ \n");
  printf("Array Len: %d \n", len);
  printf("------------------ \n");

  for(int i = 0; i < len; i++){
    printf("item at %d, with value: %s\n",i,array[i]);
  }
}

/*
  Function to fork the current process and execute the cmd passed to it
*/
void forkAndExecuteCommand(struct Command cmd){

  // process id and wait process id
  pid_t pid, waitPid;

  // the process status
  int status;

  // fork current proccess
	pid = fork();

  // if fork fails
	if (pid < 0) {
		fprintf(stderr, "ERROR: Fork failed.\n");
		exit(EXIT_FAILURE);
	}

	// child process
	if (pid == 0) {
    execvp(cmd.cmd, cmd.parameters);
		perror("ERROR: Child should never arrive here.\n");
    exit(EXIT_FAILURE);
	}
	// parent process
	else {
    // wait for child to finish
    waitPid = wait(&status);
		if (waitPid == -1) {
			fprintf(stderr, "ERROR: Waitpid failed.\n");
			exit(EXIT_FAILURE);
		}
  }
}


// /*
//   Function to redirect input to file
// */
// void redirectInputToFile(int fd[]){
//   fd = open(fd[0], O_RDONLY);
//   dup2(fd, STDIN_FILENO);
//   close(fd);
// }
//
// /*
//   Function to redirect output to file
// */
// void redirectOutputToFile(int fd){
//   fd = create(fd[1] , 0644) ;
//   dup2(fd, STDOUT_FILENO);
//   close(fd);
// }
