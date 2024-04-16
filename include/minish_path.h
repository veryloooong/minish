#pragma once

#include <stdbool.h>

#include "../include/main.h"

int minish_path_list(char **args, int *exit_status);
int minish_path_add(char **args, int *exit_status);
int minish_path_remove(char **args, int *exit_status);

char *minish_path_find(char *file);
void minish_path_init(int use_env);
void minish_path_destroy(void);
