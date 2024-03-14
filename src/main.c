#include "../include/minish_run.h"

int main(void) {
  int exit_status = 0;

  minish_main_loop(&exit_status);

  return exit_status;
}
