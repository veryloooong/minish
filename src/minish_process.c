#define _XOPEN_SOURCE 700

#include "../include/minish_process.h"

#include <dirent.h>
#include <ftw.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#include "../include/main.h"
#include "../include/minish_run.h"

int string_ends_with(const char *str, const char *suffix) {
  int str_len = strlen(str);
  int suffix_len = strlen(suffix);

  return (str_len >= suffix_len) && (0 == strcmp(str + (str_len - suffix_len), suffix));
}

int string_is_int(const char *str) {
  for (int i = 0; str[i] != '\0'; i++) {
    if (str[i] < '0' || str[i] > '9') {
      return 0;
    }
  }

  return 1;
}

void consume_first_arg(char **args) {
  int i = 1;
  while (args[i] != NULL) {
    args[i - 1] = args[i];
    i++;
  }
  args[i - 1] = NULL;
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
  if (args[1] == NULL) {
    fprintf(stderr, "minish: bg: missing argument\n");
    *exit_status = 1;
    return 1;
  } else {
    pid_t pid = fork();
    if (pid == 0) {
      consume_first_arg(args);
      minish_execute(args, exit_status);
      return 0;
    } else if (pid < 0) {
      perror("minish");
      *exit_status = 1;
      return 1;
    } else {
      waitpid(pid, NULL, WNOHANG);
      return 1;
    }
  }
}

int minish_process_ls(char **args __UNUSED, int *exit_status) {
  pid_t ppid = getpid();

  char str[50];

  sprintf(str, "pgrep -P %d -l", ppid);

  system(str);

  *exit_status = 0;
  return 1;
}

int minish_process_send_signal(char **args, int *exit_status, int signal) {
  if (args[1] == NULL) {
    fprintf(stderr, "minish: %s: missing argument\n", args[0]);
    *exit_status = 1;
    return 1;
  } else {
    if (!string_is_int(args[1])) {
      fprintf(stderr, "minish: %s: invalid argument\n", args[0]);
      *exit_status = 1;
      return 1;
    }
    pid_t pid = atoi(args[1]);
    if (pid == 0) {
      fprintf(stderr, "minish: %s: invalid argument\n", args[0]);
      *exit_status = 1;
      return 1;
    } else {
      pid_t ppid = getpid();
      pid_t pgid = getpgid(ppid);

      if (getpgid(pid) != pgid) {
        fprintf(stderr, "minish: %s: process is not a child of the current process\n", args[0]);
        *exit_status = 1;
        return 1;
      } else {
        kill(pid, signal);
        *exit_status = 0;
        return 1;
      }
    }
  }
}

int minish_process_kill(char **args, int *exit_status) {
  minish_process_send_signal(args, exit_status, SIGKILL);

  waitpid(atoi(args[1]), NULL, 0);

  return 1;
}

int minish_process_stop(char **args, int *exit_status) {
  return minish_process_send_signal(args, exit_status, SIGSTOP);
}

int minish_process_continue(char **args, int *exit_status) {
  return minish_process_send_signal(args, exit_status, SIGCONT);
}
