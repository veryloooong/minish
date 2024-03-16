#pragma once

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

typedef struct builtin_t {
  char *name;
  int (*func)(char **, int *);
} builtin_t;

extern builtin_t minish_builtins[];

// static const char *minish_builtin_strs[] = {"cd",   "dir",  "pwd",
//                                             "help", "exit", "clear"};

int minish_num_builtins(void);

// static int (*minish_builtin_funcs[])(char **, int *) = {
//     &minish_cd,   &minish_dir,  &minish_pwd,
//     &minish_help, &minish_exit, &minish_clear,
// };
