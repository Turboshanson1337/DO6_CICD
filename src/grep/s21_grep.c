#define _GNU_SOURCE
#include "s21_grep.h"

#include <getopt.h>
#include <regex.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int main(int argc, char* argv[]) {
  opt options = {0};
  if (argc < 3) {
    fprintf(stderr,
            "error: usage: s21_grep -eivclnhso [file] -f [source_file]");
  } else {
    parser(argc, argv, &options);
  }
  return 0;
}

int parser(int argc, char* argv[], opt* options) {
  int opt = 0;
  char tmp_str[4096] = {0};
  char search_str[7][4096] = {0};
  char file_name[4096] = {0};
  int option_index = 0;
  int arg_counter = 0, e_flag_counter = 1;
  static struct option long_options[] = {{0, 0, 0, 0}};
  if (argc > 2) {
    while ((opt = getopt_long(argc, argv, ":eivclnhsfo", long_options,
                              &option_index)) != -1) {
      switch (opt) {
        case 'e':
          options->e = 1;
          strncpy(search_str[e_flag_counter], argv[optind], 4095);
          argv[optind] = "§±§±";
          arg_counter++;
          e_flag_counter++;
          break;
        case 'i':
          options->i = 1;
          break;
        case 'v':
          options->v = 1;
          break;
        case 'c':
          options->c = 1;
          break;
        case 'l':
          options->l = 1;
          break;
        case 'n':
          options->n = 1;
          break;
        case 'h':
          options->h = 1;
          break;
        case 's':
          options->s = 1;
          break;
        case 'f':
          options->f = 1;
          strncpy(file_name, argv[optind], 4095);
          argv[optind] = "§±§±";
          break;
        case 'o':
          options->o = 1;
          break;
        default:
          options->error = 1;
      }
    }
  }
  if (arg_counter > 5) {
    fprintf(stderr, "error: max 5 '-e' allowed\n");
    options->error = 1;
  } else {
    int counter = 0;
    int files_count = 0;
    if (!options->e && !options->f) {
      strncpy(search_str[1], argv[optind], 4095);
      argv[optind] = "§±§±";
    }
    files_count = files_counter(argc, argv, files_count);
    int str_count = 0;
    if (options->f) {
      str_count = file_strings_counter(file_name);
      if (str_count > 3) {
        fprintf(stderr, "error: file must contain 3 or less strings\n");
        options->error = 1;
      } else {
        int i = arg_counter + 1;
        arg_counter = arg_counter + str_count;
        for (int l = i; l <= arg_counter; l++) {
          strncpy(search_str[l],
                  read_from_file(file_name, str_count, tmp_str, counter), 4095);
          counter++;
        }
      }
    }
    if (argc > 2) {
      reader(options, search_str, &argv[optind], arg_counter, files_count);
    }
  }
  return options->error;
}

void reader(opt* options, char search_str[][4096], char* argv[],
            int arg_counter, int files_count) {
  regex_t regex;
  int regflag = REG_EXTENDED;
  int success = 5;
  int change_counter = 0;
  if (options->i) {
    regflag = REG_ICASE;
  }
  optind = 0;
  char file_text[4096] = {0};
  char find_str[4096] = {0};
  while (argv[optind] != NULL) {
    FILE* f = NULL;
    f = fopen(argv[optind], "r");
    if (f) {
      regcomp(&regex, search_str[1], regflag);
      int str_count = 1;
      int match_count = 0;
      while (fgets(file_text, 4095, f) != NULL) {
        if (arg_counter > 1 && change_counter == 1) {
          strncpy(search_str[1], find_str, 4095);
        }
        success = regexec(&regex, file_text, 0, NULL, 0);

        if (arg_counter > 1) {
          strncpy(find_str, search_str[1], 4095);

          strncpy(
              search_str[6],
              arg_work(arg_counter, search_str, regflag, success, file_text),
              4095);

          strncpy(search_str[1], search_str[6], 4095);

          if (strcasestr(search_str[1], find_str) == NULL) {
            change_counter = 1;
            success = 0;
          }
        }
        int len = strlen(search_str[1]);
        char* istr = strcasestr(file_text, search_str[1]);
        int o = istr - file_text;
        if (success == 0 && !options->v) {
          match_count++;
          int stop = non_vc_printer(options, argv, optind, istr, file_text,
                                    search_str, o, files_count, len, str_count);
          if (stop == 1) {
            break;
          }
        }
        if (success == REG_NOMATCH && options->v) {
          int stop = option_v_printer(options, files_count, argv, optind,
                                      str_count, file_text);
          if (stop == 1) {
            break;
          }
          match_count++;
        }
        str_count++;
      }
      if (options->c) {
        option_c_printer(options, argv, optind, match_count, files_count);
      }
      regfree(&regex);
      fclose(f);
    }
    if (f == NULL) {
      err_file(argv, optind, options);
    }
    print_new_line(file_text, success, options, argv, optind);
    optind++;
  }
}

char* arg_work(int arg_counter, char search_str[][4096], int regflag,
               int success, char* file_text) {
  int arg[5] = {0};
  regex_t regex2;
  for (int i = 2; i <= arg_counter; i++) {
    regcomp(&regex2, search_str[i], regflag);
    int success2 = 5;
    success2 = regexec(&regex2, file_text, 0, NULL, 0);
    if (success2 == 0) {
      arg[i - 1] = 1;
    }
    regfree(&regex2);
  }
  if (success == REG_NOMATCH &&
      (arg[1] == 1 || arg[2] == 1 || arg[3] == 1 || arg[4] == 1)) {
    success = 0;

    for (int i = 2; i <= 5; i++) {
      if (arg[i - 1] == 1) {
        strncpy(search_str[1], search_str[i], 4095);
      }
    }
  }

  return search_str[1];
}

int file_strings_counter(char* file_name) {
  int counter = 0;
  char file_text[4096] = {0};
  FILE* f = NULL;
  f = fopen(file_name, "r");
  if (f) {
    while (fgets(file_text, 4095, f) != NULL) {
      counter++;
    }
    fclose(f);
  }
  return counter;
}

char* read_from_file(char* file_name, int str_count, char* tmp_str,
                     int counter) {
  char file_text[4096] = {0};
  FILE* f = NULL;
  f = fopen(file_name, "r");
  int tmp1 = 0;
  if (f) {
    while (fgets(file_text, 4095, f) != NULL || tmp1 < 3) {
      for (int i = 0; i <= counter; i++) {
        if (i == counter && i == tmp1) {
          strncpy(tmp_str, file_text, 4095);
          if (str_count > i + 1) {
            tmp_str[strlen(tmp_str) - 1] = '\0';
          }
        }
      }
      tmp1++;
    }
    fclose(f);
  }
  return tmp_str;
}

int files_counter(int argc, char* argv[], int files_count) {
  char str[4][10] = {".txt", ".c", ".h", "Makefile"};
  for (int j = 0; j < 4; j++) {
    for (int i = 0; i < argc; i++) {
      if (strcasestr(argv[i], str[j])) {
        files_count++;
      }
    }
  }
  return files_count;
}

int non_vc_printer(opt* options, char* argv[], int optind, char* istr,
                   char* file_text, char search_str[][4096], int o,
                   int files_count, int len, int str_count) {
  int stop = 0;
  if (!options->c) {
    if (!options->n) {
      if (!options->l) {
        if (!options->o) {
          if ((files_count > 1) && !options->h) {
            printf("%s:%s", argv[optind], file_text);
          } else {
            printf("%s", file_text);
          }
        } else {
          while (istr != NULL) {
            for (int i = 0; i < len; i++) {
              if (files_count > 1 && !options->h && i == 0) {
                printf("%s:", argv[optind]);
              }
              printf("%c", file_text[o + i]);
              file_text[o + i] = ' ';
            }
            printf("\n");
            istr = strcasestr(file_text, search_str[1]);
            o = istr - file_text;
          }
        }
      } else {
        printf("%s\n", argv[optind]);
        stop = 1;
      }
    } else {
      if (!options->l) {
        if (!options->o) {
          if ((files_count > 1) && !options->h) {
            printf("%s:%d:%s", argv[optind], str_count, file_text);
          } else {
            printf("%d:%s", str_count, file_text);
          }
        } else {
          int j = 0;
          while (istr != NULL) {
            for (int i = 0; i < len; i++) {
              if (files_count > 1 && !options->h && i == 0) {
                printf("%s:", argv[optind]);
              }
              if (j == 0) {
                printf("%d:%c", str_count, file_text[o + i]);
                j++;
              } else {
                printf("%c", file_text[o + i]);
              }
              file_text[o + i] = ' ';
            }
            printf("\n");
            istr = strcasestr(file_text, search_str[1]);
            o = istr - file_text;
          }
        }
      } else {
        printf("%s\n", argv[optind]);
        stop = 1;
      }
    }
  }
  return stop;
}

int option_v_printer(opt* options, int files_count, char* argv[], int optind,
                     int str_count, char* file_text) {
  int stop = 0;
  if (!options->c) {
    if (!options->n) {
      if (!options->l) {
        if ((files_count > 1) && !options->h) {
          printf("%s:%s", argv[optind], file_text);
        } else {
          printf("%s", file_text);
        }
      } else {
        printf("%s\n", argv[optind]);
        stop++;
      }
    } else {
      if (!options->l) {
        if ((files_count > 1) && !options->h) {
          printf("%s:%d:%s", argv[optind], str_count, file_text);
        } else {
          printf("%d:%s", str_count, file_text);
        }
      } else {
        printf("%s\n", argv[optind]);
        stop++;
      }
    }
  }
  return stop;
}

void option_c_printer(opt* options, char* argv[], int optind, int match_count,
                      int files_count) {
  char str[4][10] = {".txt", ".c", ".h", "Makefile"};
  for (int i = 0; i < 4; i++) {
    if (strcasestr(argv[optind], str[i])) {
      if (!options->l) {
        if ((files_count > 1) && !options->h) {
          printf("%s:%d\n", argv[optind], match_count);
        } else {
          printf("%d\n", match_count);
        }
      } else {
        if (match_count > 0) {
          if ((files_count > 1) && !options->h) {
            printf("%s:1\n%s\n", argv[optind], argv[optind]);
          } else {
            printf("1\n%s\n", argv[optind]);
          }
        } else {
          if ((files_count > 1) && !options->h) {
            printf("%s:0\n", argv[optind]);
          } else {
            printf("0\n");
          }
        }
      }
    }
  }
}

void print_new_line(char* file_text, int success, opt* options, char* argv[],
                    int optind) {
  char ch = '\n';
  if (argv[optind] != NULL) {
    if (success == 0 && strchr(file_text, ch) == NULL && !options->c &&
        !options->l && !options->v) {
      printf("\n");
    }
    if (success == REG_NOMATCH && strchr(file_text, ch) == NULL &&
        !options->c && !options->l && options->v) {
      printf("\n");
    }
  }
}

void err_file(char* argv[], int optind, opt* options) {
  char str[4][10] = {".txt", ".c", ".h", "Makefile"};
  int strcmp = 0;
  for (int i = 0; i < 4; i++) {
    if (strcasestr(argv[optind], str[i])) {
      strcmp = 1;
    }
  }
  if (!options->s && strcmp) {
    fprintf(stderr, "grep: No such file or directory\n");
  }
}
