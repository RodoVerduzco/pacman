#ifndef FATAL_ERROR_H
#define FATAL_ERROR_H

#include <stdio.h>
#include <stdlib.h>

void fatalError(const char * message);
void fatalErrorMsg(char *function, char *message);

#endif  /* NOT FATAL_ERROR_H */
