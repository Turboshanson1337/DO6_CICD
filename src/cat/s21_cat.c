#include "s21_cat.h"

#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char* argv[]) {
  opt options = {0};
  if (argc < 2) {
    fprintf(stderr, "error: usage: s21_cat -benstvET [file]");
  } else {
    parser(argc, argv, &options);
    if (!options.error) {
      reader(argv, &options);
    }
  }
  return 0;
}

void parser(int argc, char* argv[], opt* options) {
  int opt;
  int option_index;
  static struct option long_options[] = {{"number-nonblank", 0, 0, 'b'},
                                         {"number", 0, 0, 'n'},
                                         {"squeeze-blank", 0, 0, 's'},
                                         {"show-tabs", 0, 0, 't'},
                                         {"show-tabs", 0, 0, 'T'},
                                         {"show-ends", 0, 0, 'e'},
                                         {"show-nonprinting", 0, 0, 'v'},
                                         {"show-ends", 0, 0, 'E'},
                                         {0, 0, 0, 0}};
  while ((opt = getopt_long(argc, argv, "+benstvET", long_options,
                            &option_index)) != -1) {
    switch (opt) {
      case 'b':
        options->b = 1;
        break;
      case 'e':
        options->e = 1;
        options->v = 1;
        break;
      case 't':
        options->t = 1;
        options->v = 1;
        break;
      case 'n':
        options->n = 1;
        break;
      case 's':
        options->s = 1;
        break;
      case 'v':
        options->v = 1;
        break;
      case 'E':
        options->e = 1;
        break;
      case 'T':
        options->t = 1;
        break;
      default:
        options->error = 1;
    }
  }
}

void reader(char* argv[], opt* options) {
  while (argv[optind] != NULL) {
    FILE* f = fopen(argv[optind], "r");
    if (f) {
      int cur;
      int prev = 0;
      int counter = 0;
      int tmp = 1;
      int file_start = 1;
      while ((cur = fgetc(f)) != EOF) {
        if (options->s) {
          if (cur == '\n') {
            counter++;
          } else {
            counter = 0;
          }
        }
        if (cur == '\n' && (!options->s || counter < 3)) {
          if (options->n && (prev == '\n' || file_start == 1) &&
              options->b == 0) {
            printf("%6d\t", tmp++);
          }
          if (options->e) printf("$");
          printf("%c", cur);
        }
        if (cur != '\n') {
          if ((prev == '\n' || file_start == 1) && (options->b || options->n)) {
            printf("%6d\t", tmp++);
          }
          if (cur <= 31 && cur >= 0 && cur != '\t' && options->v) {
            printf("^%c", cur + 64);
          } else {
            if (cur == 127 && options->v) {
              printf("^?");
            } else {
              if (cur == '\t' && options->t) {
                printf("^I");
              } else {
                printf("%c", cur);
              }
            }
          }
        }
        prev = cur;
        file_start = 0;
      }
      fclose(f);
    } else {
      fprintf(stderr, "s21_cat: No such file or directory\n");
    }
    optind++;
  }
}
