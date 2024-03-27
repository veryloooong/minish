#pragma once

#include "../include/main.h"
#include <stdbool.h>

int minish_path_list(char **args, int *exit_status);
int minish_path_add(char **args, int *exit_status);
int minish_path_remove(char **args, int *exit_status);

void minish_path_init(int use_env);
void minish_path_destroy(void);
