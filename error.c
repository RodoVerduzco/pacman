#include "error.h"

void fatalError(const char * message)
{
    perror(message);
    exit(EXIT_FAILURE);
}

void fatalErrorMsg(char *function, char *message)
{
  printf("\n\nError in function: %s\n", function);
  printf("%s\n", message);
  printf("\nThe program will terminate.\n\n");
}
