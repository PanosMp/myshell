/*
*  Panagiotis Mparmpagiannis (AM: 3110124)
*  Tsifrikas Georgios (AM: 3110205)
*/

// include
#include "mysh-common.h"

// implementing mysh2
// supports commands with parameters.
int main(int argc, char** argv) {
  int currentShellID = 2;
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
    // print the struct
    printCommandStruct(c);
    // fork and execute the command
    forkAndExecuteCommandWithRedirect(c,currentShellID);
    // free allocated memory
    free(collection.store);
    // free(cmd);
  }

  // exit successfully
  exit(EXIT_SUCCESS);
}
