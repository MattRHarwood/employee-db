#include <getopt.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

#include "../include/common.h"
#include "../include/file.h"
#include "../include/parse.h"

void print_usage(char *filename) {

  printf("usage: %s [-h help] [-n newfile] [-f filename]\n", filename);
}

int main(int argc, char *argv[]) {

  if (argc < 2) {
    print_usage(argv[0]);
    return STATUS_ERROR;
  }

  int ch = 0;
  bool newfile = false;
  char *filepath = NULL;
  struct db_header_t *db_header = NULL;

  while (((ch = getopt(argc, argv, "hnf:")) != STATUS_ERROR)) {
    switch (ch) {
      case 'h':
        print_usage(argv[0]);
        break;
      case 'n':
        newfile = true;
        break;
      case 'f':
        filepath = optarg;
        break;
      case '?':
        printf("Unknown option -%c\n", ch);
        print_usage(argv[0]);
        break;
      default:
      return STATUS_ERROR;
    }
  }

  if (filepath == NULL) {
    printf("filepath is a required argument\n");
    return STATUS_ERROR;
  }

  int fd = -1;

  if (newfile != true) {
    fd = open_db_file(filepath);
    if (fd == -1) {
      printf("could not open db file\n");
      return STATUS_ERROR;
    }
    if (validate_db_header(fd, &db_header)) {
      printf("invalid db header\n");
      free(db_header);
      return STATUS_ERROR;
      }
  }
  else {
    fd = create_db_file(filepath);
    if (fd == STATUS_ERROR) {
      printf("could not open db file\n");
      return STATUS_ERROR;
    }
    if (create_db_header(fd, &db_header) == STATUS_ERROR) {
      printf("could not create db header\n");
      return STATUS_ERROR;
    }
    if (fd == STATUS_ERROR) {
      printf("could not open db file\n");
      return STATUS_ERROR;
    }
  }
  output_db_file(fd, db_header);
  close(fd);
  free(db_header);
  return STATUS_SUCCESS;
}
