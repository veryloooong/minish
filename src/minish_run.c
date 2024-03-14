#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

#include "../include/colors.h"
#include "../include/minish_builtins.h"
#include "../include/minish_readline.h"
#include "../include/minish_run.h"

int minish_run_process(char **args, int *status __attribute__((unused))) {
  pid_t pid;
  int status_code;

  pid = fork();
  if (pid < 0) {
    // Error forking
    perror("minish");
  } else if (pid == 0) {
    // Child process
    if (execvp(args[0], args) == -1) {
      perror("minish");
    }
    exit(EXIT_SUCCESS);
  } else {
    do {
      waitpid(pid, &status_code, WUNTRACED);
    } while (!WIFEXITED(status_code) && !WIFSIGNALED(status_code));
  }

  return 1;
}

int minish_execute(char **args, int *exit_status) {
  if (args[0] == NULL) {
    return 1;
  }

  // Check if the command is a built-in
  for (int i = 0; i < minish_num_builtins(); i++) {
    if (strcmp(args[0], minish_builtin_strs[i]) == 0) {
      return (*minish_builtin_funcs[i])(args, exit_status);
    }
  }

  return minish_run_process(args, exit_status);
}

void minish_main_loop(int *exit_status) {
  char *line;
  char **args;
  int run_state;

  char *status_color = *exit_status == 0 ? COLOR_GREEN_BOLD : COLOR_RED_BOLD;

  do {
    printf("minish %s>%s ", status_color, COLOR_RESET);
    line = minish_read_line();
    fprintf(stderr, "Input:\t%s\n", line);
    args = minish_make_args(line);
    fprintf(stderr, "Args:\t%s\n", args[0]);
    run_state = minish_execute(args, exit_status);

    free(line);
    free(args);
  } while (run_state);
}
