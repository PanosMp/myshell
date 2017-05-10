/*
*  < AUTHOR >
*/

// mysh1

// include
#include "mysh-common.h"

// implementing mysh1
// supports commands without parameters
// however if parameters are passed the
// code will execute normally as if it
// was meant to handle commands with parameters.
int main(int argc, char** argv) {

  // struct to hold the string array of the splitted command
  struct StringArray collection;
  // struct to hold the command in proper form
  struct Command c;
  // var to store the command;
  char * cmd;

  // read user input
  while((cmd = readCommand(1)) != NULL){
    // trim the whitespaces from the command
    cmd = trimwhitespace(cmd);
    // assign the splitted command to the collection
    collection = splitStrByDelim(cmd, SPACE_DELIM);
    // transform the array to command struct
    c = seperateCmdParams(collection);
    // fork and execute the command
    forkAndExecuteCommand(c);
    // free allocated memory
    free(collection.store);
    free(cmd);
  }

  // exit successfully
  exit(EXIT_SUCCESS);
}
