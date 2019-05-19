#include "types.h"
#include "user.h"
#include "fcntl.h"
#include "param.h"

#define TESTS 20

// increase two-digit decimal number
void inc(char *s, int pos) {
  if (s[pos+1] == '9') {
    s[pos+1] = '0';
    s[pos]++;
  } else {
    s[pos+1]++;
  }
}

int main(int argc, char **argv) 
{
  if (argc > 1) {
    printf(1, "Test passed: %s\n", argv[argc-1]);
    exit();
  }
  
  char file_name[] = "00";
  char text_zero[] = "!#shebangtest\n";
  char text[] = "!#00\n";

  for (int i = 0; i < TESTS; ++i) {
    int fd = open(file_name, O_CREATE | O_RDWR);
    if (fd == 0) {
        printf(1, "open() error\n");
        exit();
    }

    if (i == 0) {
      write(fd, text_zero, strlen(text_zero));
    } else {
      write(fd, text, strlen(text));
      inc(text, 2);
    }

    close(fd);
    int pid = fork();
    if (pid < 0) {
      printf(1, "fork() error\n");
      exit();
    }
    if (pid == 0) {
      char *argv[MAXARG];
      argv[0] = file_name;
      argv[1] = 0;
      int err = exec(file_name, argv);
      if (err < 0 ){
        if (i >= MAXDEPTH) {
            printf(1, "Test passed: %s\n", file_name);
        } else {
            printf(1, "exec() error\n");
        }
      }
      exit();
    }
    wait();
    inc(file_name, 0);
  }
  exit();
}
