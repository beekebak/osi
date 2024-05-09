#include <stdio.h>
#include <errno.h>
#include "common_lib.h"

void PrintAndExit(char* error){
  perror(error);
  exit(EXIT_FAILURE);
}

void PrintErrnoAndExit(){
  PrintAndExit(strerror(errno));
}
