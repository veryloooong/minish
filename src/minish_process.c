#define _XOPEN_SOURCE 700

#include "../include/minish_process.h"

#include <dirent.h>
#include <ftw.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#include "../include/main.h"
#include "../include/minish_run.h"

void consume_first_arg(char **args) {
  int i = 1;
  while (args[i] != NULL) {
    args[i - 1] = args[i];
    i++;
  }
  args[i - 1] = NULL;
}

static void sigchld_handle(int signum __UNUSED, siginfo_t *siginfo, void *unused __UNUSED) {
  int status;

  if (waitpid(siginfo->si_pid, &status, 0) == -1 || !WIFEXITED(status)) {
    perror("minish");
  }
}

int minish_process_run_fg(char **args, int *exit_status) {
  if (args[1] == NULL) {
    fprintf(stderr, "minish: fg: missing argument\n");
    *exit_status = 1;
    return 1;
  } else {
    consume_first_arg(args);
    return minish_execute(args, exit_status);
  }
}

int minish_process_run_bg(char **args, int *exit_status) {
  pid_t pid = fork();
  // int status;
  // pid_t wait;

  if (pid == 0) {
    consume_first_arg(args);
    *exit_status = minish_execute(args, exit_status);
    exit(*exit_status);
  } else if (pid < 0) {
    fprintf(stderr, "minish: error forking\n");
    *exit_status = 1;
  } else {
    struct sigaction sa;
    sa.sa_flags = SA_SIGINFO;
    sa.sa_sigaction = sigchld_handle;
    sigemptyset(&sa.sa_mask);

    if (sigaction(SIGCHLD, &sa, NULL) == -1) {
      perror("minish");
      *exit_status = 1;
    }
  }

  return 1;
}

int minish_process_ls(char **args __UNUSED, int *exit_status) {
  pid_t ppid = getpid();

  char str[64];

  sprintf(str, "ps -o pid= -o comm= --ppid %d", ppid);

  *exit_status = system(str);

  return 1;
}
int minish_process_kill(char **args, int *exit_status) {
  // TODO

  return 1;
}
int minish_process_stop(char **args, int *exit_status) {
  // TODO

  return 1;
}
int minish_process_continue(char **args, int *exit_status) {
  // TODO

  return 1;
}
