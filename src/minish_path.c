#include "../include/minish_path.h"

#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "../include/main.h"

// Helper function prototypes

Path *path_new(char *path);
PathList *path_list_new(void);
PathList *get_paths_from_env(void);
char *minish_path_get_str(void);
void minish_path_init(int use_env);
void minish_path_destroy(void);
void path_list_append(PathList *paths, char *path);
bool path_list_remove(PathList *paths, char *path);
void path_list_free(PathList *paths);
char *make_absolute_path(char *dir, char *file);

// Global variables

static PathList *path_list = NULL;

// Main function implementations

int minish_path_list(char **args __UNUSED, int *exit_status) {
  Path *current = path_list->head;

  while (current != NULL) {
    printf("%s%c", current->path, current->next == NULL ? '\n' : ':');
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

  path_list_append(path_list, args[1]);
  *exit_status = 0;
  return 1;
}

int minish_path_remove(char **args, int *exit_status) {
  if (args[1] == NULL) {
    fprintf(stderr, "minish: pathrm: missing argument\n");
    *exit_status = 1;
    return 1;
  }

  if (!path_list_remove(path_list, args[1])) {
    fprintf(stderr, "minish: pathrm: path not found\n");
    *exit_status = 1;
    return 1;
  }

  *exit_status = 0;
  return 1;
}

char *minish_path_find(char *file) {
  Path *current = path_list->head;

  while (current != NULL) {
    char *absolute_path = make_absolute_path(current->path, file);
    if (access(absolute_path, F_OK) != -1) {
      return absolute_path;
    }
    free(absolute_path);
    current = current->next;
  }

  return NULL;
}

// Remember to free after use
char *minish_path_get_str(void) {
  Path *current = path_list->head;
  size_t len = 0;

  while (current != NULL) {
    len += strlen(current->path) + 1;
    current = current->next;
  }

  char *path_str = (char *)malloc(len + 1);
  char *path_str_ptr = path_str;

  current = path_list->head;

  while (current != NULL) {
    size_t path_len = strlen(current->path);
    strncpy(path_str_ptr, current->path, path_len);
    path_str_ptr += path_len;
    *path_str_ptr = ':';
    path_str_ptr++;
    current = current->next;
  }

  *(path_str_ptr - 1) = '\0';

  return path_str;
}

void minish_path_init(int use_env) {
  if (path_list != NULL) {
    minish_path_destroy();
  }

  if (use_env) {
    path_list = get_paths_from_env();
  } else {
    path_list = path_list_new();
  }
}

void minish_path_destroy(void) {
  path_list_free(path_list);
  path_list = NULL;
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
  pl->tail = NULL;
  return pl;
}

PathList *get_paths_from_env(void) {
  PathList *paths = path_list_new();
  char *path = getenv("PATH");
  char *path_copy = (char *)malloc(strlen(path) + 1);
  strcpy(path_copy, path);
  char *path_token = strtok(path_copy, ":");
  while (path_token != NULL) {
    path_list_append(paths, path_token);
    path_token = strtok(NULL, ":");
  }
  free(path_copy);
  return paths;
}

void path_list_append(PathList *paths, char *path) {
  Path *new_path = path_new(path);
  if (paths->head == NULL) {
    paths->head = new_path;
  } else {
    paths->tail->next = new_path;
  }
  paths->tail = new_path;
}

bool path_list_remove(PathList *paths, char *path) {
  Path *current = paths->head;
  Path *prev = NULL;

  size_t path_len = strlen(path);

  while (current != NULL) {
    if (strncmp(current->path, path, path_len) == 0) {
      // Found the path to remove
      if (prev == NULL) {
        paths->head = current->next;  // Removing the head
      } else {
        prev->next = current->next;  // Removing from the middle or end
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

char *make_absolute_path(char *dir, char *file) {
  size_t dirlen = strlen(dir);
  size_t filelen = strlen(file);

  char *absolute_path = malloc(strlen(dir) + strlen(file) + 10);
  strncpy(absolute_path, dir, dirlen + 1);
  strncat(absolute_path, "/", 2);
  strncat(absolute_path, file, filelen + 1);
  return absolute_path;
}
