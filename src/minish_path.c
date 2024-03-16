#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../include/main.h"
#include "../include/minish_path.h"

// Helper function prototypes

Path *path_new(char *path);
PathList *path_list_new(void);
PathList *get_paths_from_env(void);
void path_list_push(PathList *paths, char *path);
bool path_list_remove(PathList *paths, char *path);
void path_list_free(PathList *paths);

// Main function implementations

int minish_path_list(char **args __UNUSED, int *exit_status) {
  Path *current = path_list->head;

  while (current != NULL) {
    printf("%s\n", current->path);
    current = current->next;
  }

  *exit_status = 0;
  return 1;
}

int minish_path_add(char **args, int *exit_status) {
  if (args[1] == NULL) {
    fprintf(stderr, "minish: pathadd: missing argument\n");
    *exit_status = 1;
    return 1;
  }

  path_list_push(path_list, args[1]);
  *exit_status = 0;
  return 1;
}

// Helper function implementations

Path *path_new(char *path) {
  Path *p = (Path *)malloc(sizeof(Path));
  size_t len = strlen(path);
  p->path = (char *)malloc(len + 1);
  strncpy(p->path, path, len + 1);
  p->next = NULL;
  return p;
}

PathList *path_list_new(void) {
  PathList *pl = malloc(sizeof(PathList));
  pl->head = NULL;
  return pl;
}

PathList *get_paths_from_env(void) {
  PathList *paths = path_list_new();
  char *path = getenv("PATH");
  char *path_copy = (char *)malloc(strlen(path) + 1);
  strcpy(path_copy, path);
  char *path_token = strtok(path_copy, ":");
  while (path_token != NULL) {
    path_list_push(paths, path_token);
    path_token = strtok(NULL, ":");
  }
  free(path_copy);
  return paths;
}

void path_list_push(PathList *paths, char *path) {
  Path *new_path = path_new(path);
  new_path->next = paths->head;
  paths->head = new_path;
}

bool path_list_remove(PathList *paths, char *path) {
  Path *current = paths->head;
  Path *prev = NULL;

  size_t path_len = strlen(path);

  while (current != NULL) {
    if (strncmp(current->path, path, path_len) == 0) {
      // Found the path to remove
      if (prev == NULL) {
        paths->head = current->next; // Removing the head
      } else {
        prev->next = current->next; // Removing from the middle or end
      }
      free(current->path);
      free(current);
      return true;
    }
    prev = current;
    current = current->next;
  }

  return false;
}

void path_list_free(PathList *paths) {
  Path *current = paths->head;
  while (current != NULL) {
    Path *next = current->next;
    free(current->path);
    free(current);
    current = next;
  }
  free(paths);
}
