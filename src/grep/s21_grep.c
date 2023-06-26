#include "s21_grep.h"
int main(int argc, char *argv[]) {
  char pattern[4096];
  Flags flags = {0};
  grep_parser(&argc, argv, pattern, &flags);
  output(argc, argv, pattern, flags);
}
void grep_parser(int *argc, char *argv[], char pattern[], Flags *flags) {
  int opt;
  while ((opt = getopt_long(*argc, argv, "e:ivclnhs", 0, 0)) != -1) {
    switch (opt) {
      case 'e':
        flags->eF = 1;
        if (!pattern[0]) {
          strcat(pattern, optarg);
        } else {
          strcat(pattern, "|");
          strcat(pattern, optarg);
        }
        break;
      case 'i':
        flags->iF = 1;
        break;
      case 'v':
        flags->vF = 1;
        break;
      case 'c':
        flags->cF = 1;
        break;
      case 'l':
        flags->lF = 1;
        break;
      case 'n':
        flags->nF = 1;
        break;
      case 'h':
        flags->hF = 1;
        break;
      case 's':
        flags->sF = 1;
        break;
    }
  }
  if (flags->eF && optind + 1 < *argc) {
    flags->count_files = 1;
  }
  if (optind + 2 < *argc) {
    flags->count_files = 1;
  }
  if (flags->lF) {
    flags->vF = 0;
  }
}

void output(int argc, char *argv[], char pattern[], Flags flags) {
  char ch[4096];
  int flag = REG_EXTENDED, len = 0, count_str = 0, stopper = 0,
      count_str_nF = 1, result = 0;
  regex_t regex;
  if (flags.iF) {
    flag = REG_ICASE;
  }
  if (flags.eF) {
    regcomp(&regex, pattern, flag);
  } else {
    regcomp(&regex, argv[optind], flag);
    optind++;
  }
  for (; optind < argc; optind++) {
    FILE *file = fopen(argv[optind], "r");
    if (file != NULL) {
      while (fgets(ch, 4095, file) != NULL && stopper == 0) {
        count_str = print_regex(count_str, &result, regex, len, argv, &stopper,
                                &count_str_nF, ch, flags);
        count_str_nF++;
      }
      if (flags.cF) {
        print_only_str(argv, &count_str, flags);
      }
      stopper = 0;
      count_str_nF = 1;
      fclose(file);
    }
    if (file == NULL && flags.sF == 0) {
      printf("s21_grep: %s: No such file or directory\n", argv[optind]);
    }
  }
  regfree(&regex);
}
int print_regex(int count_str, int *result, regex_t regex, int len,
                char *argv[], int *stopper, int *count_str_nF, char *ch,
                Flags flags) {
  *result = regexec(&regex, ch, 0, NULL, 0);
  len = strlen(ch);
  if (ch[len - 1] == '\n') {
    ch[len - 1] = '\0';
  }
  if (*result) {
    print_vF(argv, ch, flags);
  }
  if (!(*result) && flags.vF == 0) {
    if (flags.lF) {
      printf("%s\n", argv[optind]);
      *stopper = 1;
    }
    if (*stopper == 0) {
      print_match(argv, stopper, count_str_nF, ch, flags);
    }
    count_str++;
  }
  if (flags.vF && *result) {
    count_str++;
  }
  return count_str;
}
void print_match(char *argv[], int *stopper, int *count_str_nF, char *ch,
                 Flags flags) {
  if (flags.nF && *stopper == 0) {
    if (flags.count_files) {
      printf("%s:", argv[optind]);
    }
    printf("%d:%s\n", *count_str_nF, ch);
  }
  if (flags.hF && flags.cF == 0) {
    printf("%s\n", ch);
  }
  if (flags.count_files && !flags.cF && !flags.nF && !flags.hF) {
    printf("%s:", argv[optind]);
  }
  if (!flags.cF && !flags.nF && !flags.hF && flags.oF == 0) {
    printf("%s\n", ch);
  }
}
void print_only_str(char *argv[], int *count_str, Flags flags) {
  if (flags.count_files && flags.hF == 0) {
    printf("%s:", argv[optind]);
  }
  printf("%d\n", *count_str);
  *count_str = 0;
}
void print_vF(char *argv[], char *ch, Flags flags) {
  if (flags.vF) {
    if (flags.count_files && flags.cF == 0) {
      printf("%s:", argv[optind]);
    }
    if (flags.cF == 0) {
      printf("%s\n", ch);
    }
  }
}