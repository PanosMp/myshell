/*
*  < AUTHOR >
*/

// mysh1

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

    // read command cat < in.txt > out.txt || cat < in.txt >> out.txt

    // assign the splitted command to the collection
    collection = splitStrByDelim(cmd, SPACE_DELIM);
    // transform the array to command struct
    c = seperateCmdParams(collection);
    // fork and execute the command
    // forkAndExecuteCommand(c, currentShellID);
    forkAndExecuteCommandWithRedirect(c, currentShellID);
    // free allocated memory
    free(collection.store);
    free(cmd);
  }

  // exit successfully
  exit(EXIT_SUCCESS);
}
