#include <stdio.h>
#include <stdlib.h>

#include "../include/argparse.h"
#include "../include/minish_path.h"
#include "../include/minish_run.h"

const char *usages[] = {
    "minish [OPTION]...",
    NULL,
};

int verbose = 0;

int main(int argc, const char **argv) {
  int use_env = 0;

  struct argparse_option options[] = {
      OPT_HELP(),
      // OPT_GROUP("Basic options:"),
      OPT_BOOLEAN('e', "use-env", &use_env, "Use the system's PATH environment variable", NULL, 0,
                  0),
      OPT_BOOLEAN('v', "verbose", &verbose, "Print extra debug information", NULL, 0, 0),
      OPT_END(),
  };

  struct argparse argparse;
  argparse_init(&argparse, options, usages, 0);
  argparse_describe(&argparse, "A minimal shell implementation", NULL);
  argc = argparse_parse(&argparse, argc, argv);

  minish_path_init(use_env);

  // TODO: Implement verbose mode
  int status = minish_main_loop();

  minish_path_destroy();

  return status;
}
