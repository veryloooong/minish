#include "minish_run.h"

int main() {
  int exit_status = 0;

  minish_main_loop(&exit_status);

  return exit_status;
}