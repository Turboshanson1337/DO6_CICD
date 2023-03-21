#ifndef SRC_GREP_S21_GREP_H_
#define SRC_GREP_S21_GREP_H_

typedef struct options {
  int e;
  int i;
  int v;
  int c;
  int l;
  int n;
  int h;
  int s;
  int f;
  int o;
  int error;
} opt;

int parser(int argc, char* argv[], opt* options);
void reader(opt* options, char search_str[][4096], char* argv[],
            int arg_counter, int files_count);
char* arg_work(int arg_counter, char search_str[][4096], int regflag,
               int success, char* file_text);
void print_new_line(char* file_text, int success, opt* options, char* argv[],
                    int optind);
int file_strings_counter(char* file_name);
char* read_from_file(char* file_name, int str_count, char* tmp_str,
                     int counter);
int files_counter(int argc, char* argv[], int files_count);
int non_vc_printer(opt* options, char* argv[], int optind, char* istr,
                   char* file_text, char search_str[][4096], int o,
                   int files_count, int len, int str_count);
int option_v_printer(opt* options, int files_count, char* argv[], int optind,
                     int str_count, char* file_text);
void option_c_printer(opt* options, char* argv[], int optind, int match_count,
                      int files_count);
void err_file(char* argv[], int optind, opt* options);

#endif  //  SRC_GREP_S21_GREP_H_
