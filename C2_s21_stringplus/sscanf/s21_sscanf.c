#include <stdarg.h>

#include "../s21_string.h"
#include "parser.h"
#include "s_handlers.h"

union result {
  long int *ld_p;
  int *d_p;
  short int *hd_p;
  unsigned long int *ul_p;
  unsigned int *u_p;
  unsigned short int *uh_p;
  long double *Lf_p;
  double *lf_p;
  float *f;
  char *c_p;
  char *s_p;
  void **p_p;
} out;

void va_set(union data data, struct format_options opt, va_list vl) {
  if (opt.miss) {
    return;
  }
  switch (opt.type) {
    case 'n':
      out.d_p = va_arg(vl, int *);
      *out.d_p = (int)data.lint_t;
      break;
    case 'i':
    case 'd':
      if (opt.length & LONG) {
        out.ld_p = va_arg(vl, long int *);
        *out.ld_p = (long int)data.lint_t;
      } else if (opt.length & SHORT) {
        out.hd_p = va_arg(vl, short int *);
        *out.hd_p = (short int)data.lint_t;
      } else {
        out.d_p = va_arg(vl, int *);
        *out.d_p = (int)data.lint_t;
      }
      break;
    case 'x':
    case 'X':
    case 'o':
    case 'u':
      if (opt.length & LONG) {
        out.ul_p = va_arg(vl, unsigned long int *);
        *out.ul_p = (unsigned long int)data.lint_t;
      } else if (opt.length & SHORT) {
        out.uh_p = va_arg(vl, unsigned short int *);
        *out.uh_p = (unsigned short int)data.lint_t;
      } else {
        out.u_p = va_arg(vl, unsigned int *);
        *out.u_p = (unsigned int)data.lint_t;
      }
      break;
    case 'c':
      out.c_p = va_arg(vl, char *);
      *out.c_p = (char)data.lint_t;
      break;
    case 'p':
      out.p_p = va_arg(vl, void **);
      *out.p_p = (void *)data.lint_t;
      break;
    case 's':
      out.s_p = va_arg(vl, char *);
      s21_strcpy(out.s_p, (char *)data.string_t);
      break;
    case 'f':
    case 'e':
    case 'E':
    case 'g':
    case 'G':
      if (opt.length & LONGDOUBLE) {
        out.Lf_p = va_arg(vl, long double *);
        *out.Lf_p = (long double)data.ldouble_t;
      } else if (opt.length & LONG) {
        out.lf_p = va_arg(vl, double *);
        *out.lf_p = (double)data.ldouble_t;
      } else if (opt.length & SHORT || opt.length == 0) {
        out.f = va_arg(vl, float *);
        *out.f = (float)data.ldouble_t;
      }
      break;
    default:
      // unknown specifier
      break;
  }
}

int s21_sscanf(char *str, const char *format, ...) {
  s21_size_t succes_counter = 0;
  union data data = {0};
  va_list vl;
  int read_counter = 0;

  va_start(vl, format);

  while (*format) {
    while (isblank(*str)) {
      str++; /* идём до первого непробельного символа в строке */
      ++read_counter;
      continue;
    }
    while (isblank(*format)) {
      format++; /* идём до первого непробельного символа в формате */
      continue;
    }
    if (*str == '\0' && *format == '\0') break;

    if (*format == *str) {
      if (*(format + 1) == '%' && *str == '%') {
        /* если в форматной строке %% и в str % */
        ++format;
      }
      ++format;
      ++str;
      ++read_counter;
      continue;
    }

    struct format_options options = {0};
    if (*format == '%') {
      ++format;
      options = parse_scanformat_options(&format);

      /* (считываем из str данные в соответсвии с типом в options) И (записываем
       * в union data) */
      if (scan_fmtunit(&str, &data, options, &read_counter)) {
        if (options.type != 'n' && !options.miss) ++succes_counter;
      } else {
        break; /* выход при ошибке считывания */
      }

      /* считываем указатель на переменную и записываем значение в соответствии
       * с типом */
      va_set(data, options, vl);
      continue;
    }

    if (*format != *str) {
      break;
    }
  }

  va_end(vl);
  return succes_counter;
}
