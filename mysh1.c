/*
*  < AUTHOR >
*/

// mysh1

// include
#include "mysh-common.h"


int main(int argc, char** argv) {

  // var to store the command;
  char * cmd;
  struct StringArray collection;
  const char delimSpace[2] = " ";

  while( (cmd = readCommand(1)) != NULL ){
    // printf("FINALLY: %s\n", cmd);
    // collection = splitStrByDelim(cmd, delimSpace);
    collection = splitStrByDelim(cmd, delimSpace);
    printArray(collection.store, collection.size);


    // free allocated stuff
    free(collection.store);
    free(cmd);
  }


  // forkAndExecuteCommand();

  return 0;
}
