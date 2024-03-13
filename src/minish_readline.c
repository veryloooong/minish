#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

#include "minish_readline.h"

#define CHUNK_SIZE 256UL
char *minish_read_line(void) {
  char *input = NULL;
  int ch;
  size_t input_len = 0;

  input = realloc(input, sizeof(*input) * CHUNK_SIZE);
  if (!input) {
    return NULL;
  }

  while ((ch = fgetc(stdin)) != EOF && ch != '\n') {
    input[input_len++] = ch;
    if (input_len % CHUNK_SIZE == 0) {
      input = realloc(input, sizeof(*input) * (input_len + CHUNK_SIZE));
      if (!input) {
        return NULL;
      }
    }
  }

  input[input_len] = '\0';
  input = realloc(input, sizeof(*input) * (input_len + 1));

  return input;
}

char **minish_tokenize_line(char *line) {
  // TODO: Implement this function
  return NULL;
}
