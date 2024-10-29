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
  bool listEmployees = false;
  char *filepath = NULL;
  char *employeeDataRaw = NULL;
  char *removeEmployee = NULL;
  struct db_header_t *db_header = NULL;
  struct employee_t *employees = NULL;

  while (((ch = getopt(argc, argv, "hnf:a:lr:")) != STATUS_ERROR)) {
    switch (ch) {
      case 'h':
        print_usage(argv[0]);
        break;
      case 'n':
        newfile = true;
        break;
      case 'l':
        listEmployees = true;
        break;
      case 'f':
        filepath = optarg;
        break;
      case 'a':
         employeeDataRaw = optarg;
        break;
      case 'r':
         removeEmployee = optarg;
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

    if (validate_db_header(fd, &db_header) != STATUS_SUCCESS) {
      printf("could not validate db header\n");
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

  if (read_employees(fd, db_header, &employees) != STATUS_SUCCESS) {
    printf("could not read employees\n");
    free(employees);
    return STATUS_ERROR;
  }

  if (employeeDataRaw) {
    db_header->count++;
    employees = realloc(employees, db_header->count*sizeof(struct employee_t));
    db_header->filesize += sizeof(struct employee_t);

    if (add_employee(db_header, employees, employeeDataRaw) != STATUS_SUCCESS) {
    printf("couldn't add employee\n");
    return STATUS_ERROR;
    }
  }

  if (removeEmployee) {
    int deleted = remove_employee(db_header, employees, removeEmployee);
    db_header->count--;
    employees = realloc(employees, db_header->count*sizeof(struct employee_t));
    db_header->filesize -= sizeof(struct employee_t);
  }

  if (listEmployees) {
    list_employees(db_header, employees);
  }

  output_db_file(fd, db_header, employees);

  close(fd);
  free(db_header);
  free(employees);
  return STATUS_SUCCESS;
}
