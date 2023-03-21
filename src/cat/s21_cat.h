#ifndef SRC_CAT_S21_CAT_H_
#define SRC_CAT_S21_CAT_H_

typedef struct options {
  int b;
  int e;
  int n;
  int s;
  int t;
  int v;
  int error;
} opt;

void no_args(int fd);
void parser(int argc, char* argv[], opt* options);
void reader(char* argv[], opt* options);

#endif  //  SRC_CAT_S21_CAT_H_
