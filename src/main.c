/**
 * A minimal shell for Project 1, IT3070
 */

#include <stdio.h>
#include <stdlib.h>

#include "minish_readline.h"

void minish_loop(void) {
  char *line;
  char **args;
  int status = 0;

  // TODO: loop handling
  do {
    printf("> ");
    line = minish_read_line();
    args = minish_tokenize_line(line);

    free(line);
    free(args);
  } while (status);
}

int main() {
  // TODO: Implement a minimal shell loop
  while (1) {
  }
}