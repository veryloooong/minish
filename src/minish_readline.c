#include <ctype.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

#include "../include/minish_readline.h"

#define CHUNK_SIZE 256UL
char *minish_read_line(void) {
  char *input = NULL;
  int ch;
  size_t input_len = 0;
  size_t input_cap = CHUNK_SIZE;

  input = realloc(input, sizeof(*input) * input_cap);
  if (!input) {
    return NULL;
  }

  while ((ch = getchar()) != EOF && ch != '\n') {
    input[input_len++] = ch;
    if (input_len >= input_cap) {
      input_cap += CHUNK_SIZE;
      input = realloc(input, sizeof(*input) * input_cap);
      if (!input) {
        return NULL;
      }
    }
  }

  input[input_len] = '\0';
  input = realloc(input, sizeof(*input) * (input_len + 1));

  return input;
}

char *unescape_token(char *token) {
  char *in = token;
  char *out = token;

  while (*in) {
    if ((in[0] == '\\') && (in[1] == '"')) {
      *out++ = '"';
      in += 2;
    } else {
      *out++ = *in++;
    }
  }

  *out = '\0';
  return token;
}

char *tokenize(char *str, char **next) {
  char *current = str;
  char *start = str;
  bool is_quoted = false;

  // Clear spaces
  while (*current && isspace(*current))
    current++;
  start = current;

  // Check if the token is quoted
  if (*current == '"') {
    is_quoted = true;
    current++;

    start = current;
    while (1) {
      // Go until we find a quote or the end of string.
      while (*current && (*current != '"'))
        current++;

      // End of string
      if (*current == '\0')
        goto finalize;

      // Check if the quote is escaped
      if (*(current - 1) == '\\') {
        current++;
        continue;
      }

      // Reached the ending quote.
      goto finalize;
    }
  }

  // If the token is not quoted, go until we find a space or the end of string
  while (*current && !isspace(*current))
    current++;

// Finalize the token
finalize:
  if (*current) {
    *current = '\0';
    current++;

    // Clear spaces
    while (*current && isspace(*current))
      current++;
  }
  *next = current;

  return is_quoted ? unescape_token(start) : start;
}

// TODO fix this
#define TOKEN_CHUNK_SIZE 64UL
char **minish_make_args(char *line) {
  size_t token_buffer_cap = TOKEN_CHUNK_SIZE;
  size_t token_buffer_len = 0UL;
  char **token_buffer = malloc(token_buffer_cap * sizeof(*token_buffer));

  if (!token_buffer) {
    return NULL;
  }

  char *current = line;
  char *token = tokenize(current, &current);

  while (*token) {
    token_buffer[token_buffer_len++] = token;

    if (token_buffer_len >= token_buffer_cap) {
      token_buffer_cap += TOKEN_CHUNK_SIZE;
      token_buffer =
          realloc(token_buffer, token_buffer_cap * sizeof(*token_buffer));
      if (!token_buffer) {
        return NULL;
      }
    }

    token = tokenize(current, &current);
  }

  token_buffer[token_buffer_len] = NULL;

  return token_buffer;
}
