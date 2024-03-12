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
int minish_cd(char **args);
// ls
int minish_ls(char **args);
// pwd
int minish_pwd(char **args);
// help
int minish_help(char **args);
// exit
int minish_exit(char **args);
// clear
int minish_clear(char **args);

#endif