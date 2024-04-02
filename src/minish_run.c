#include "../include/minish_run.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

#include "../include/colors.h"
#include "../include/minish_builtins.h"
#include "../include/minish_path.h"
#include "../include/minish_readline.h"

int minish_run_process(char **args, int *exit_status) {
  pid_t pid;
  int status_code;

  pid = fork();
  if (pid < 0) {
    perror("minish");
    *exit_status = 1;
  } else if (pid == 0) {
    // TODO: Make this run from our own path instead of the system's
    char *executable = minish_path_find(args[0]);
    if (executable != NULL) {
      args[0] = executable;
      // fprintf(stderr, "minish: using %s\n", executable);
    }
    if (execve(args[0], args, NULL) == -1) {
      perror("minish");
    }
    *exit_status = 1;
    exit(EXIT_FAILURE);
  } else {
    do {
      waitpid(pid, &status_code, WUNTRACED);
    } while (!WIFEXITED(status_code) && !WIFSIGNALED(status_code));

    *exit_status = WEXITSTATUS(status_code);
  }

  return 1;
}

int minish_execute(char **args, int *exit_status) {
  if (args[0] == NULL) {
    return 1;
  }

  // Check if the command is a built-in
  for (int i = 0, num_builtins = minish_num_builtins(); i < num_builtins; i++) {
    if (strcmp(args[0], minish_builtins[i].name) == 0) {
      return minish_builtins[i].func(args, exit_status);
    }
  }

  return minish_run_process(args, exit_status);
}

int minish_main_loop(void) {
  int operation_status = 0;

  char *line;
  char **args;
  int run_state = 1;

  do {
    char *status_color = operation_status == 0 ? COLOR_GREEN_BOLD : COLOR_RED_BOLD;
    printf("minish %s>%s ", status_color, COLOR_RESET);
    line = minish_read_line();
    args = minish_make_args(line);
    run_state = minish_execute(args, &operation_status);

    free(line);
    free(args);
  } while (run_state);

  return operation_status;
}
