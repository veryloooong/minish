#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "../include/colors.h"
#include "../include/minish_builtins.h"

/**
 * Changes the current working directory.
 * If no argument is provided, changes to the home directory.
 * If an argument is provided, changes to the specified directory.
 *
 * @param args The command arguments. args[0] is the command itself, args[1] is
 * the directory path.
 * @param exit_status Pointer to the exit status variable.
 * @return 1 to continue executing the shell.
 */
int minish_cd(char **args, int *exit_status) {
  if (args[1] == NULL) {
    char *home_dir = getenv("HOME");
    if (home_dir == NULL) {
      fprintf(stderr, "lsh: dir: No $HOME variable set\n");
      *exit_status = 1;
    } else {
      if (chdir(home_dir) != 0) {
        perror("minish");
        *exit_status = 1;
      } else {
        *exit_status = 0;
      }
    }
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

int minish_dir(char **args, int *exit_status) {
  char *directory = NULL;
  if (args[1] == NULL) {
    directory = ".";
  } else {
    directory = args[1];
  }

  DIR *d;
  struct dirent *dir;
  d = opendir(directory);
  if (d) {
    while ((dir = readdir(d)) != NULL) {
      switch (dir->d_type) {
      case 4:
        printf(COLOR_BLUE_BOLD "\t%s/\n" COLOR_RESET, dir->d_name);
        break;
      case 10:
        printf(COLOR_GREEN_ITALIC "\t%s\n" COLOR_RESET, dir->d_name);
        break;
      default:
        printf("\t%s\n", dir->d_name);
        break;
      }
    }
    closedir(d);
    *exit_status = 0;
  } else {
    perror("minish");
    *exit_status = 1;
  }

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
