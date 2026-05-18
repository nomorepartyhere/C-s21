#include "s_handlers.h"

#include "../s21_string.h"

void s21_putc(const char ch, char *out) {
  const char temp[2] = {ch, '\0'};
  s21_strcat(out, temp);
}

char *s21_strcat(char *dest, const char *src) {
  if (dest == s21_NULL || src == s21_NULL) return s21_NULL;

  char *p = dest;
  while (*p != '\0') p++;

  while (*src != '\0') *p++ = *src++;

  *p = '\0';
  return dest;
}

static s21_size_t get_lint(char *str, long int *result, int base) {
  char *err;
  *result = strtol(str, &err, base);

  s21_size_t dist = err - str;
  if (dist == 0) {
    dist = s21_strlen(str);
  }
  return dist;
}

static s21_size_t get_ulint(char *str, long int *result, int base) {
  char *err;
  *result = strtoul(str, &err, base);

  s21_size_t dist = err - str;
  if (dist == 0) {
    dist = s21_strlen(str);
  }
  return dist;
}

static s21_size_t get_ldouble(char *str, long double *result) {
  char *err;
  *result = strtold(str, &err);

  s21_size_t dist = err - str;
  if (dist == 0) {
    dist = s21_strlen(str);
  }
  return dist;
}

static int detect_base(char *str) {
  if (*str == '0') {
    if (*(str + 1) == 'x' || *(str + 1) == 'X') {
      return 16;
    }
    return 8;
  }
  return 10;
}

bool scan_fmtunit(char **str, union data *data, struct format_options opt,
                  int *read_counter) {
  int base = 0;
  data->lint_t = 0;
  int char_count = 0;
  switch (opt.type) {
    case 'd':
      char_count = get_lint(*str, &data->lint_t, 10);
      break;
    case 'u':
      char_count = get_ulint(*str, &data->lint_t, 10);
      break;
    case 'i':
      base = detect_base(*str);
      char_count = get_ulint(*str, &data->lint_t, base);
      break;
    case 'n':
      data->lint_t = *read_counter;
      break;
    case 'x':
    case 'X':
    case 'p':
      char_count = get_lint(*str, &data->lint_t, 16);
      break;
    case 'o':
      char_count = get_lint(*str, &data->lint_t, 8);
      break;
    case 'c':
      data->lint_t = **str;
      char_count = 1;
      break;
    case 's':
      while (isprint(**str) && !isblank(**str)) {
        s21_putc(**str, data->string_t);
        ++(*str);
      }
      break;
    case 'f':
    case 'e':
    case 'E':
    case 'g':
    case 'G':
      char_count = get_ldouble(*str, &data->ldouble_t);
      break;
    default:
      // unknown specifier
      break;
  }

  if (data->lint_t == 0 &&
      (char_count != 1 && char_count != 2)) { /* ошибка считывания */
    return 0;
  }
  *str += char_count;
  *read_counter += char_count;

  return 1;
}
