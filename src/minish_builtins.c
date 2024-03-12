#include "minish_builtins.h"

#include <stdio.h>

#define EXECUTE_SUCCESS 0
#define EXECUTE_FAILURE 1

// List of builtin functions
int (*minish_builtin_funcs[])(char **) = {&minish_cd,   &minish_ls,
                                          &minish_pwd,  &minish_help,
                                          &minish_exit, &minish_clear};

int minish_cd(char **args) {
  printf("unimplemented\n");
  return 1;
}

int minish_ls(char **args) {
  printf("unimplemented\n");
  return 1;
}

int minish_pwd(char **args) {
  printf("unimplemented\n");
  return 1;
}

int minish_help(char **args) {
  printf("minish - minimal shell\n\n");
  printf("Builtins:\n");

  // cd
  printf("cd\t\tChange the current working directory\n");
  // ls
  printf("ls\t\tList files in the current directory\n");
  // pwd
  printf("pwd\t\tPrint the current working directory\n");
  // help
  printf("help\t\tDisplay this help message\n");
  // clear
  printf("clear\t\tClear the terminal\n");
  // exit
  printf("exit\t\tExit the shell\n");

  // Non builtins and executables
  printf("\nNon builtins and executables:\n");
  printf("Any other command will be executed as a program in a new process\n");

  // Path variables
  printf("\nPath variables:\n");
  printf("path\t\tPrint the list of directories to search for executables\n");
  printf("addpath\t\tAdd a directory to the list of directories to search for "
         "executables\n");

  return EXECUTE_SUCCESS;
}

// Always return 0 to indicate success
int minish_exit(char **args) { return EXECUTE_SUCCESS; }

int minish_clear(char **args) {
  printf("\033[H\033[J");
  return EXECUTE_SUCCESS;
}