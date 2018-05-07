/*
  A simple library to handle errors.
  Written by Ludovic Cyril Michel.
*/

#ifndef ERROR_H
#define ERROR_H

// Support Libraries
#include <stdio.h>
#include <stdlib.h>

// Function Declaration
void print_network_error(const char *command, int critical);
void print_usage_error(const char *program, const char *params);
void print_fork_error();

#endif  /* ERROR_H */
