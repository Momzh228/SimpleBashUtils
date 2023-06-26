#include "s21_cat.h"

int main(int argc, char *argv[]) {
  Flags flags = {0, 0, 0, 0, 0, 0};
  cat_parser(&argc, argv, &flags);
  output(argc, argv, flags);
}
void cat_parser(int *argc, char *argv[], Flags *flags) {
  int opt;
  int long_ingex = 0;
  struct option const long_options[] = {
      {"number-nonblank", no_argument, NULL, 'b'},
      {"number", no_argument, NULL, 'n'},
      {"squeeze-blank", no_argument, NULL, 's'}};
  while ((opt = getopt_long(*argc, argv, "bevEnstT", long_options,
                            &long_ingex)) != -1) {
    switch (opt) {
      case 'b':
        flags->bF = 1;
        break;
      case 'e':
        flags->vF = 1;
        flags->eF = 1;
        break;
      case 'v':
        flags->vF = 1;
        break;
      case 'E':
        flags->eF = 1;
        break;
      case 'n':
        flags->nF = 1;
        break;
      case 's':
        flags->sF = 1;
        break;
      case 't':
        flags->vF = 1;
        flags->tF = 1;
        break;
      case 'T':
        flags->tF = 1;
        break;
    }
    if (flags->bF == 1) {
      flags->nF = 0;
    }
  }
}
void output(int argc, char *argv[], Flags flags) {
  char prev_char, prev_prev_char;
  int str_count = 1, count_s, ch;
  for (; optind < argc; optind++) {
    FILE *file = fopen(argv[optind], "rb");
    if (file != NULL) {
      while ((ch = fgetc(file)) != EOF) {
        if (flags.sF) {
          count_s = squeeze_blank(&ch, &prev_char, &prev_prev_char);
          if (count_s != 0) {
            continue;
          }
        }
        if (flags.bF && ((prev_char == '\n' && ch != '\n') ||
                         (str_count == 1 && ch != '\n'))) {
          printf("%6d\t", str_count);
          str_count++;
        }
        if (flags.nF && (prev_char == '\n' || str_count == 1)) {
          printf("%6d\t", str_count);
          str_count++;
        }
        if (flags.tF || flags.eF || flags.vF) {
          hide_sym(&ch, &flags);
        }
        prev_prev_char = prev_char;
        prev_char = ch;
        putchar(ch);
      }
      fclose(file);
    } else {
      printf("s21_cat: %s: No such file or directory\n", argv[optind]);
    }
    prev_char = -1;
  }
}
int squeeze_blank(int *ch, char *prev_char, char *prev_prev_char) {
  int count = 0;
  if (*ch == '\n' && *prev_char == '\n' &&
      (*prev_prev_char == '\n' || *prev_prev_char == '\0')) {
    count++;
  }
  return count;
}
void hide_sym(int *ch, Flags *flags) {
  if (flags->eF && *ch == '\n') {
    printf("$");
  }
  if (flags->tF && *ch == '\t') {
    printf("^");
    *ch = 'I';
  }

  if (*ch < 32 && *ch != '\n' && *ch != '\t') {
    printf("^");
    *ch += 64;
  }
  if (*ch == 127) {
    printf("^");
    *ch -= 64;
  }
  if (*ch > 127) {
    printf("M-");
    if (*ch >= 160 && *ch < 255) {
      *ch -= 128;
    } else {
      printf("^");
      *ch = *ch - 64;
    }
  }
}