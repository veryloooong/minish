#include "minish_builtins.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

// List of builtin functions
int (*minish_builtin_funcs[])(char **, int *) = {&minish_cd,   &minish_ls,
                                                 &minish_pwd,  &minish_help,
                                                 &minish_exit, &minish_clear};

int minish_cd(char **args, int *exit_status) {
  if (args[1] == NULL) {
    chdir(getenv("HOME"));
    *exit_status = 0;
  } else {
    if (chdir(args[1]) != 0) {
      perror("minish");
      *exit_status = 1;
    } else {
      *exit_status = 0;
    }
  }
  return 1;
}

int minish_ls(char **args, int *exit_status) {
  if (args[1] == NULL) {
    // TODO: do ls in the current directory
  } else {
    // TODO: do ls in the specified directory
  }
  printf("unimplemented\n");
  *exit_status = 1;
  return 1;
}

int minish_pwd(char **args __attribute__((unused)), int *exit_status) {
  char cwd[1024];

  if (getcwd(cwd, sizeof(cwd)) != NULL) {
    printf("%s\n", cwd);
    *exit_status = 0;
  } else {
    perror("minish");
    *exit_status = 1;
  }

  return 1;
}

int minish_help(char **args __attribute__((unused)), int *exit_status) {
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

  *exit_status = 0;

  return 1;
}

// Always return 0 to signal the shell to exit
int minish_exit(char **args, int *exit_status) {
  if (args[1] != NULL) {
    *exit_status = atoi(args[1]);
  } else {
    *exit_status = 0;
  }
  return 0;
}

int minish_clear(char **args __attribute__((unused)), int *exit_status) {
  printf("\033[H\033[J");
  *exit_status = 0;
  return 1;
}