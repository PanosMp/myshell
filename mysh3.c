/*
*  < AUTHOR >
*/

// mysh1

// include
#include "mysh-common.h"

// implementing mysh3
// supports commands with parameters and std in-out redirections.
int main(int argc, char** argv) {
  int currentShellID = 3;
  // struct to hold the string array of the splitted command
  struct StringArray collection;
  // struct to hold the command in proper form
  struct Command c;
  // var to store the command;
  char * cmd;

  // read user input
  while((cmd = readCommand(currentShellID)) != NULL){
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
    printCommandStruct(c);
    forkAndExecuteCommandWithRedirect(c, currentShellID);
    // free allocated memory
    free(collection.store);
    // free(cmd);
  }

  // exit successfully
  exit(EXIT_SUCCESS);
}
