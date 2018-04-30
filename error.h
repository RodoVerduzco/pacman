#ifndef ERROR_H
#define ERROR_H

#include <stdio.h>
#include <stdlib.h>

void fatalError(const char * message);
void fatalErrorMsg(char *function, char *message);

#endif  /* NOT ERROR_H */
