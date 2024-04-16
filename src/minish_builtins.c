#include "../include/minish_builtins.h"

#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "../include/colors.h"
#include "../include/main.h"
#include "../include/minish_path.h"
#include "../include/minish_process.h"

builtin_t minish_builtins[] = {{"cd", minish_cd},
                               {"ls", minish_ls},
                               {"pwd", minish_pwd},
                               {"help", minish_help},
                               {"clear", minish_clear},
                               {"exit", minish_exit},
                               {"pathls", minish_path_list},
                               {"pathadd", minish_path_add},
                               {"pathrm", minish_path_remove},
                               {"procls", minish_process_ls},
                               {"prockill", minish_process_kill},
                               {"procstop", minish_process_stop},
                               {"proccont", minish_process_continue}};

int minish_num_builtins(void) { return sizeof(minish_builtins) / sizeof(builtin_t); }

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
      fprintf(stderr, "lsh: dir: no $HOME variable set\n");
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

/**
 * Lists the contents of a directory.
 *
 * @param args The command arguments. The second (`args[1]`) argument is the directory path. If
 * unspecified, defaults to the current directory.
 * @param exit_status A pointer to the exit status variable.
 * @return 1 indicating successful execution.
 */
int minish_ls(char **args, int *exit_status) {
  char *directory = (args[1] == NULL) ? "." : args[1];

  DIR *d;
  struct dirent *dir;
  d = opendir(directory);

  if (d) {
    while ((dir = readdir(d)) != NULL) {
      if (dir->d_name[0] == '.') {
        continue;
      }
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
    *exit_status = closedir(d);
  } else {
    perror("minish");
    *exit_status = 1;
  }

  return 1;
}

int minish_pwd(char **args __UNUSED, int *exit_status) {
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

int minish_help(char **args __UNUSED, int *exit_status) {
#define BOLD_COMMAND(x) COLOR_GREEN_BOLD x COLOR_RESET

  printf("\nminish - minimal shell\n\n");
  printf("Builtins:\n");

  // Builtins
  printf(BOLD_COMMAND("cd") "\t\tChange the current working directory\n");
  printf(BOLD_COMMAND("ls") "\t\tList files in the current directory\n");
  printf(BOLD_COMMAND("pwd") "\t\tPrint the current working directory\n");
  printf(BOLD_COMMAND("help") "\t\tDisplay this help message\n");
  printf(BOLD_COMMAND("clear") "\t\tClear the terminal\n");
  printf(BOLD_COMMAND("exit") "\t\tExit the shell\n");

  // Non builtins and executables
  printf("\nNon builtins and executables:\n");
  printf("Any other command will be executed as a program in a new process\n");

  // Path variables
  printf("\nPath variables:\n");
  printf(BOLD_COMMAND("pathls") "\t\tPrint the list of directories to search "
                                "for executables\n");
  printf(BOLD_COMMAND("pathadd") "\t\tAdd a directory to the list of directories to search for "
                                 "executables\n");
  printf(BOLD_COMMAND("pathrm") "\t\tRemove a directory from the list of directories to search "
                                "for executables\n");

  // Process management
  printf("\nProcess management:\n");
  printf(BOLD_COMMAND("bg") "\t\tRun a process in the background\n");
  printf(BOLD_COMMAND("fg") "\t\tRun a process in the foreground\n");
  printf(BOLD_COMMAND("procls") "\t\tList running processes\n");
  printf(BOLD_COMMAND("prockill") "\tKill a running process\n");
  printf(BOLD_COMMAND("procstop") "\tStop a running process\n");
  printf(BOLD_COMMAND("proccont") "\tContinue a stopped process\n");

  *exit_status = 0;

  return 1;
}

// Always return 0 to signal the shell to exit
int minish_exit(char **args, int *exit_status) {
  if (args[1] != NULL) *exit_status = atoi(args[1]);

  return 0;
}

int minish_clear(char **args __UNUSED, int *exit_status) {
  printf("\033[H\033[J");
  *exit_status = 0;
  return 1;
}
