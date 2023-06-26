#include <getopt.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
typedef struct {
  int bF;
  int eF;
  int vF;
  int nF;
  int sF;
  int tF;
} Flags;

void cat_parser(int *argc, char *argv[], Flags *flags);
void hide_sym(int *ch, Flags *flags);
int squeeze_blank(int *ch, char *prev_char, char *prev_prev_char);
void output(int argc, char *argv[], Flags flags);
