/*
*  < AUTHOR >
*/

// mysh1

// include
#include "mysh-common.h"

// implementing mysh2
// supports commands with parameters.
int main(int argc, char** argv) {

  // struct to hold the string array of the splitted command
  struct StringArray collection;
  // struct to hold the command in proper form
  struct Command c;
  // var to store the command;
  char * cmd;

  // read user input
  while((cmd = readCommand(2)) != NULL){
    // trim the whitespaces from the command
    cmd = trimwhitespace(cmd);

    // read command cat < in.txt > out.txt || cat < in.txt >> out.txt
    // assign the splitted command to the collection
    collection = splitStrByDelim(cmd, RE_IN);
    // transform the array to command struct
    c = seperateCmdParams(collection);
    // fork and execute the command
    // forkAndExecuteCommand(c);
    // free allocated memory
    free(collection.store);
    free(cmd);
  }

  // exit successfully
  exit(EXIT_SUCCESS);
}
