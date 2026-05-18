#include "parser.h"

static int length(const char **fmt) {
  int length = 0;

  switch (**fmt) {
    case 'h':
      length |= SHORT;
      ++(*fmt);
      break;
    case 'l':
      length |= LONG;
      ++(*fmt);
      break;
    case 'L':
      length |= LONGDOUBLE;
      ++(*fmt);
      break;
  }
  return length;
}

static int miss(const char **fmt) {
  bool miss = 0;

  if (**fmt == '*') {
    miss = 1;
    ++(*fmt);
  }
  return miss;
}

static int type_scan(const char **fmt) {
  char spec[] = "cdieEfgGosuxXpn%";

  for (int i = 0; spec[i]; ++i) {
    if (**fmt == spec[i]) {
      char type = **fmt;
      ++(*fmt);
      return (int)type;
    }
  }
  return '\0';
}

struct format_options parse_scanformat_options(const char **fmt) {
  struct format_options sp = {0};

  sp.miss = miss(fmt);
  sp.length = length(fmt);
  sp.type = type_scan(fmt);
  return sp;
}
