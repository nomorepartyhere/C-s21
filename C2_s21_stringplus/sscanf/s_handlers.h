#ifndef HANDLERS_H
#define HANDLERS_H

#include <stdarg.h>
#include <stdbool.h>
#include <stdlib.h>

#include "./parser.h"

#define MAXLEN 2048

union data {
  long int lint_t;
  long double ldouble_t;
  char string_t[MAXLEN];
};

union data_scan {
  long int *lint_t;
  long double *ldouble_t;
  char **string_t;
};

char *format_options(char *, union data, struct format_options);
bool scan_fmtunit(char **str, union data *data, struct format_options opt,
                  int *read_counter);

#endif  // HANDLERS_H
