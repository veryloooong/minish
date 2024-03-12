/*
 * Builtin functions for minish
 *
 * Has the following functions:
 * - cd
 * - ls
 * - pwd
 * - help
 * - exit
 */

#ifndef BUILTINS_H_IMPL
#define BUILTINS_H_IMPL

char *minish_builtin_strs[] = {"cd", "ls", "pwd", "help", "exit", "clear"};

int minish_num_builtins() {
  return sizeof(minish_builtin_strs) / sizeof(char *);
}

// cd
int minish_cd(char **args, int *exit_status);
// ls
int minish_ls(char **args, int *exit_status);
// pwd
int minish_pwd(char **args, int *exit_status);
// help
int minish_help(char **args, int *exit_status);
// exit
int minish_exit(char **args, int *exit_status);
// clear
int minish_clear(char **args, int *exit_status);

#endif