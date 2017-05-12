/*
*  Panagiotis Mparmpagiannis (AM: 3110124)
*  Tsifrikas Georgios (AM: 3110205)
*/

// include
#include "mysh-common.h"

// implementing mysh5
// supports commands with parameters and std in-out redirections and pipes.
int main(int argc, char** argv) {
  int currentShellID = 5;
  // struct to hold the pipeline
  struct PipelineStore pStore;
  // struct to hold the string array of the splitted command
  struct StringArray collection;
  // struct to hold the string array of pipes
  struct StringArray pipes;
  // struct to hold the command in proper form
  struct Command c;
  // var to store the command;
  char * cmd;

  // read user input
  while((cmd = readCommand(currentShellID)) != NULL){
    // if a pipe is found
    if(strstr(cmd, PIPE_DELIM)){

      // split the string to individual pipes
      pipes  = splitStrByDelim(cmd, PIPE_DELIM);
      // transformPipes to commands
      pStore = transformPipes(pipes);

      // debug
      // printPipeline(pStore);
      // chain the pipes
      return fork_pipes (pStore.size, pStore);
      perror("fork_pipes");
    }
    // if a pipe is not found
    else{
      // trim the whitespaces from the command
      cmd = trimwhitespace(cmd);
      // assign the splitted command to the collection
      collection = splitStrByDelim(cmd, SPACE_DELIM);
      // transform the array to command struct
      c = seperateCmdParams(collection);
      // filter file descriptors + transform c
      c = filterCommandFd(c);
      // fork and execute the command
      // forkAndExecuteCommand(c, currentShellID);
      // printCommandStruct(c);
      forkAndExecuteCommandWithRedirect(c, currentShellID);
      // free allocated memory
      free(collection.store);
      // free(cmd);
    }

    printf("Hello \n");
  }

  // exit successfully
  exit(EXIT_SUCCESS);

}
