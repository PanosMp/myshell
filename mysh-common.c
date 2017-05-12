/*
*  Panagiotis Mparmpagiannis (AM: 3110124)
*  Tsifrikas Georgios (AM: 3110205)
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
  // printf("Command: %s\n", t.cmd);
  // printArray(t.parameters, t.param_size);
  return t;
}

/*
  Function to fork the current process and execute the cmd passed to it
  - Note
    used in myshell1 + myshell2
*/
void forkAndExecuteCommand(struct Command cmd, int shellID){

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

    // execute command
    execvp(cmd.cmd, cmd.parameters);

    // ERROR
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

/*
  Fork and execute shell command. Accepts std in-out redirections
*/
void forkAndExecuteCommandWithRedirect(struct Command cmd, int shellID){

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

    // check if the stdin is redirected
    if(cmd.in > 0){
      // replace standard input with input file
      dup2(cmd.in, 0);

      // close unused file descriptor
      close(cmd.in);
    }

    // check if the stout is redirected
    if(cmd.out > 0){
      // replace standard output with output file
      dup2(cmd.out, 1);

      // close unused file descriptor
      close(cmd.out);
    }

    if(cmd.parameters[cmd.param_size - 1] != NULL && cmd.param_size > 1){
      cmd.parameters[cmd.param_size] = NULL;
    }

    // execute the command
    execvp(cmd.cmd, cmd.parameters);


    // ERROR
    perror("execvp");
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

/*
  Function to filter std in-out redirections.
*/
struct Command filterCommandFd(struct Command cmd){

  // used to store the executable command length
  // eg => ls -l > a.txt the commandLength = 2 {ls, -l}
  int commandLength = 0;

  // loop through the parameters and look for redirects
  for(int i = 0; i < cmd.param_size; i++){

    // on redirect in
    if(strcmp(cmd.parameters[i], RE_IN) == 0){
      //incr the pointer to get the in file
      i++;
      cmd.in = open(cmd.parameters[i], O_RDONLY);
      // printf("Redirecting stdin to: %s\n", cmd.parameters[i]);
      if(cmd.in < 0){
        printf("Failed to redirect input. Exiting...\n");
        exit(EXIT_FAILURE);
      }
    }
    // on redirect out
    else if(strcmp(cmd.parameters[i], RE_OUT) == 0){
      //incr the pointer to get the in file
      i++;
      cmd.out = open(cmd.parameters[i], O_WRONLY | O_TRUNC | O_CREAT , S_IRUSR | S_IRWXU);

      perror("open");
      if(cmd.out < 0){
        printf("Failed to redirect output. Exiting...\n");
        exit(EXIT_FAILURE);
      }
      // printf("Redirecting stdout to: %s\n", cmd.parameters[i]);
    }else if(strcmp(cmd.parameters[i], RA_OUT) == 0){
      //incr the pointer to get the in file
      i++;
      cmd.out = open(cmd.parameters[i], O_RDWR | O_APPEND | O_CREAT , S_IRUSR | S_IRWXU);
      // printf("Redirecting stdout_append to: %s\n", cmd.parameters[i]);
      if(cmd.out < 0){
        printf("Failed to redirect output (append_mode). Exiting...\n");
        exit(EXIT_FAILURE);
      }
    }else{
      // incr commandLength
      commandLength++;
    }
  }

  // char to keep the new parameters
  char ** newParameters = malloc(sizeof(cmd.parameters));
  // copy the N first elements
  for(int i = 0; i < commandLength; i++){
    newParameters[i] = malloc(sizeof(cmd.parameters[i]));
    newParameters[i] = cmd.parameters[i];
  }

  // assing new values
  cmd.parameters = newParameters;
  cmd.param_size = commandLength;


  return cmd;
}

/*
  Spawn proc
*/
int spawn_proc (int in, int out, struct Command cmd){

    pid_t pid;

    if ((pid = fork ()) == 0)
      {
        perror("fork");
        if (in != 0)
          {
            dup2 (in, 0);
            close (in);
          }

        if (out != 1)
          {
            dup2 (out, 1);
            close (out);
          }

        if(cmd.in > 0){
          // replace standard input with input file
          dup2(cmd.in, 0);

          // close unused file descriptor
          close(cmd.in);
        }

        // check if the stout is redirected
        if(cmd.out > 0){
          // replace standard output with output file
          dup2(cmd.out, 1);

          // close unused file descriptor
          close(cmd.out);
        }

        if(cmd.parameters[cmd.param_size - 1] != NULL && cmd.param_size > 1){
          cmd.parameters[cmd.param_size] = NULL;
        }

        // execute the command
        execvp(cmd.cmd, cmd.parameters);

        perror("dup2");
        perror("close");
        return execvp(cmd.cmd, cmd.parameters);
        perror("execvp");
      }

    printf("Pid:  %d\n", pid);
    return pid;
}

void forkAndProcessPipelines(struct Command cmd, int shellID){
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

    // check if the stdin is redirected
    if(cmd.in > 0){
      // replace standard input with input file
      dup2(cmd.in, 0);

      // close unused file descriptor
      close(cmd.in);
    }

    // check if the stout is redirected
    if(cmd.out > 0){
      // replace standard output with output file
      dup2(cmd.out, 1);

      // close unused file descriptor
      close(cmd.out);
    }

    if(cmd.parameters[cmd.param_size - 1] != NULL && cmd.param_size > 1){
      cmd.parameters[cmd.param_size] = NULL;
    }

    // execute the command
    execvp(cmd.cmd, cmd.parameters);


    // ERROR
    perror("execvp");
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

/*
  FORK PIPES
*/
int fork_pipes (int n, struct PipelineStore s){

  int i;
  // pid_t pid;
  // int status;
  int in, fd [2];

  /* The first process should get its input from the original file descriptor 0.  */
  in = 0;

  /* Note the loop bound, we spawn here all, but the last stage of the pipeline.  */
  for (i = 0; i < n - 1; ++i)
    {
      pipe (fd);
      perror("pipe");

      /* f [1] is the write end of the pipe, we carry `in` from the prev iteration.  */
      spawn_proc (in, fd [1], s.store[i]);
      perror("spawn_proc");
      /* No need for the write end of the pipe, the child will write here.  */
      close (fd [1]);
      perror("close");
      /* Keep the read end of the pipe, the next child will read from there.  */
      in = fd [0];
    }

  /* Last stage of the pipeline - set stdin be the read end of the previous pipe
     and output to the original file descriptor 1. */
  if (in != 0)
    dup2 (in, 0);

  perror("dup2");

  /* Execute the last stage with the current process. */
  return execvp(s.store[i].cmd, s.store[i].parameters);
  perror("execvp");
}

//
struct PipelineStore transformPipes(struct StringArray array){

  char * temp;
  struct PipelineStore pStore;
  struct StringArray tempArr;
  pStore.store = malloc(array.size * sizeof(struct Command));
  pStore.size = array.size;

  for(int i = 0; i < array.size; i++){

    // store the cmd trimmed
    temp = trimwhitespace(array.store[i]);

    // split the cmd by " "
    tempArr = splitStrByDelim(temp, SPACE_DELIM);

    // assign the cmd & params to store :+: filter command for redirections
    pStore.store[i] = filterCommandFd(seperateCmdParams(tempArr));
  }

  return pStore;
}

/*
  Function to print the contents of a struct Command.
*/
void printCommandStruct(struct Command c){
  printf("--------STRUCT--------\n");
  printArray(c.parameters, c.param_size);
  printf("----------------------\n");
  printf("in: %d\n", c.in);
  printf("out: %d\n", c.out);
  printf("-------END SRTUCT-----\n");
}

/*
  Function to print a char array
*/
void printArray(char ** array, int len){
  printf("---------------------\n");
  printf("Array Len: %d\n", len);
  printf("---------------------\n");

  for(int i = 0; i < len; i++){
    printf("item at %d, with value: %s\n",i,array[i]);
  }
}

/*
  Function to print a pipeline struct
*/
void printPipeline(struct PipelineStore pStore){
  printf("pStore Size: %d\n", pStore.size);
  if(pStore.size <= 0){
    printf("ERROR: Empty Pipeline\n");
  }
  for(int i = 0; i < pStore.size; i++){
    printCommandStruct(pStore.store[i]);
  }
}
