#ifndef PARSER_H
#define PARSER_H

#include <stdarg.h>
#include <stdbool.h>

enum flags { LEFT = 1, PLUS = 2, SPACE = 4, ZERO = 8, HASH = 16 };
enum length { SHORT = 1, LONG = 2, LONGDOUBLE = 4 };

struct format_options {
  char flags;
  int width;
  int precision;
  int length;
  char type;
  bool miss;
};

struct va_list_wrapper {
  va_list v;
};

struct format_options parse_format_options(const char **fmt,
                                           struct va_list_wrapper *vl);

struct format_options parse_scanformat_options(const char **fmt);

#endif  // PARSER_H
