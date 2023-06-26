#include <getopt.h>
#include <regex.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
typedef struct {
  int eF;
  int iF;
  int vF;
  int cF;
  int lF;
  int nF;
  int hF;
  int sF;
  int fF;
  int oF;
  int count_files;
} Flags;

void grep_parser(int *argc, char *argv[], char pattern[], Flags *flags);
int print_regex(int count_str, int *result, regex_t regex, int len,
                char *argv[], int *stopper, int *count_str_nF, char *ch,
                Flags flags);
void print_match(char *argv[], int *stopper, int *count_str_nF, char *ch,
                 Flags flags);
void print_only_str(char *argv[], int *count_str, Flags flags);
void print_vF(char *argv[], char *ch, Flags flags);
void output(int argc, char *argv[], char pattern[], Flags flags);