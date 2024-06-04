#include "../include/minish_run.h"

#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

#include "../include/colors.h"
#include "../include/main.h"
#include "../include/minish_builtins.h"
#include "../include/minish_path.h"
#include "../include/minish_readline.h"

int minish_run_process(char **args, int *exit_status) {
  char *shell_cmd = "/bin/sh";
  char *run_shell = malloc(strlen(shell_cmd) + 20 + strlen(args[0]));
  run_shell[0] = '\0';
  strcat(run_shell, shell_cmd);
  strcat(run_shell, " ");
  strcat(run_shell, args[0]);
  strcat(run_shell, " 2>/dev/null");

  if (system(run_shell) == 0) {
    *exit_status = 0;
    free(run_shell);
    return 1;
  }
  free(run_shell);

  pid_t pid;
  int status_code;

  pid = fork();
  if (pid < 0) {
    perror("minish");
    *exit_status = 1;
  } else if (pid == 0) {
    char *executable = minish_path_find(args[0]);
    if (executable != NULL) {
      args[0] = executable;
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

volatile int operation_status = 0;

void sigint_handler(int sig __UNUSED) {
  printf("\n");
  operation_status = 1;
  fflush(stdout);
}

int minish_main_loop(void) {
  char *line;
  char **args;
  int run_state = 1;

  do {
    signal(SIGINT, sigint_handler);
    char *status_color = operation_status == 0 ? COLOR_GREEN_BOLD : COLOR_RED_BOLD;
    char cwd[1024];
    getcwd(cwd, sizeof(cwd));
    printf("minish (%s) %s>%s ", cwd, status_color, COLOR_RESET);
    line = minish_read_line();
    args = minish_make_args(line);
    run_state = minish_execute(args, (int *)&operation_status);

    free(line);
    free(args);
  } while (run_state);

  return operation_status;
}
