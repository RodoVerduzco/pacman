/*
  A simple library to handle errors.
  Written by Ludovic Cyril Michel.
*/

#include "error.h"

void print_network_error(const char *command, int critical) {
  printf("Error running: %s\n", command);
  if (critical) {
    exit(EXIT_FAILURE);
  }
}

void print_usage_error(const char *program, const char *params) {
  printf("Wrong usage, use as: %s %s\n", program, params);
  exit(EXIT_FAILURE);
}

void print_fork_error() {
  printf("Error forking\n");
  exit(EXIT_FAILURE);
}
