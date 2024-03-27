#include <stdlib.h>

#include "../include/main.h"
#include "../include/minish_path.h"
#include "../include/minish_run.h"

int main(void) {
  minish_path_init(0);

  int status = minish_main_loop();

  minish_path_destroy();

  return status;
}
