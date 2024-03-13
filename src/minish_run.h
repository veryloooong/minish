#ifndef RUN_H_IMPL
#define RUN_H_IMPL

int minish_run_process(char **args, int *exit_status);
int minish_execute(char **args, int *exit_status);
void minish_main_loop(int *exit_status);

#endif
