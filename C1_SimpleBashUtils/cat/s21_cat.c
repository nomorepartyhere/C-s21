#include <getopt.h>
#include <stdio.h>

struct option longOptions[] = {
    {"number-nonblank", 0, 0, 'b'},
    {"number", 0, 0, 'n'},
    {"squeeze-blank", 0, 0, 's'},
    {0, 0, 0, 0},
};

typedef struct options {
  int n, b, e, t, s, v;
} options;

void flags_reading(int argc, char *argv[], options *opt);
void flags_working(char *argv[], options *opt);
void flag_n(int new_str, int *index);
void flag_b(int new_str, int *index, char symbol);
void flag_e(char symbol);
void flag_t(char *symbol);
void flag_s(char symbol, int new_str, int *empty_str);
void flag_v(char *symbol);

int main(int argc, char *argv[]) {
  if (argc > 1) {
    options opt = {0};
    flags_reading(argc, argv, &opt);
    flags_working(argv, &opt);
  }
  return 0;
}

void flags_reading(int argc, char *argv[], options *opt) {
  char flag;
  flag = getopt_long(argc, argv, "nbetsvTE", longOptions, 0);

  switch (flag) {
    case 'b':
      opt->b = 1;
      break;
    case 'n':
      opt->n = 1;
      break;
    case 's':
      opt->s = 1;
      break;
    case 'e':
      opt->e = 1;
      opt->v = 1;
      break;
    case 'E':
      opt->e = 1;
      break;
    case 't':
      opt->t = 1;
      opt->v = 1;
      break;
    case 'T':
      opt->t = 1;
      break;
    case 'v':
      opt->v = 1;
      break;
    default:
      break;
  }
}

void flags_working(char *argv[], options *opt) {
  FILE *file;
  int i = optind;
  while (argv[i]) {
    if ((file = fopen(argv[i], "r")) != NULL) {
      int new_str = 1;
      int index = 0;
      char symbol;
      int empty_str = 0;
      while ((symbol = fgetc(file)) != EOF) {
        // if(argv[i]) {
        // empty_str = 0;
        if (opt->n == 1) {
          flag_n(new_str, &index);
        }
        if (opt->b == 1) {
          flag_b(new_str, &index, symbol);
        }
        if (opt->e == 1) {
          flag_e(symbol);
          flag_v(&symbol);
        }
        if (opt->t == 1) {
          flag_t(&symbol);
          flag_v(&symbol);
        }
        if (opt->s == 1) {
          flag_s(symbol, new_str, &empty_str);
        }
        if (opt->v == 1) {
          flag_v(&symbol);
        }
        if (symbol == '\n') {
          new_str = 1;
        } else {
          new_str = 0;
        }
        if (empty_str != 2) {
          printf("%c", symbol);
        }
        // } else {
        //     printf("No such file or directory");
        // }
      }
    }
    i++;
    fclose(file);
  }
}

void flag_n(int new_str, int *index) {
  if (new_str == 1) {
    *index += 1;
    printf("%6d\t", *index);
  }
}

void flag_b(int new_str, int *index, char symbol) {
  if (new_str == 1) {
    if (symbol != '\n') {
      *index += 1;
      printf("%6d\t", *index);
    }
  }
}

void flag_e(char symbol) {
  if (symbol == '\n') {
    printf("$");
  }
}

void flag_t(char *symbol) {
  if (*symbol == '\t') {
    printf("^");
    *symbol += 64;
  }
}

void flag_s(char symbol, int new_str, int *empty_str) {
  if (symbol == '\n' && new_str == 1) {
    if (symbol == '\n' && *empty_str > 0) {
      *empty_str = 2;
    } else {
      *empty_str = 1;
    }
  } else {
    *empty_str = 0;
  }
}

void flag_v(char *symbol) {
  if (*symbol >= 0 && *symbol <= 31 && *symbol != 10 && *symbol != 9) {
    putchar('^');
    *symbol += 64;
  }
  if (*symbol == 127) {
    putchar('^');
    *symbol = 63;
  }
}
