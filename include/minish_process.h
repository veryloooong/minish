#pragma once

int minish_process_run_fg(char **args, int *exit_status);
int minish_process_run_bg(char **args, int *exit_status);
int minish_process_ls(char **args, int *exit_status);
int minish_process_kill(char **args, int *exit_status);
int minish_process_stop(char **args, int *exit_status);
int minish_process_continue(char **args, int *exit_status);
