#include <stdio.h>
#include <unistd.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <string.h>

#include "../include/common.h"
#include "../include/parse.h"

int output_db_file(int fd, struct db_header_t *header, struct employee_t *employees) {

  if (fd < 0) {
    printf("invalid file descriptor to output\n");
    return STATUS_ERROR;
  }

  //shorten file if needed
  ftruncate(fd, header->filesize);

  int count = header->count;

  header->magic = htonl(header->magic);
  header->version = htons(header->version);
  header->count = htons(header->count);
  header->filesize = htonl(header->filesize);

  lseek(fd, 0, SEEK_SET);

  write(fd, header, sizeof(struct db_header_t));

  for (int i = 0; i < count; i++) {
    employees[i].hours = htons(employees[i].hours);
  }

  write(fd, employees, count * sizeof(struct employee_t));

  return STATUS_SUCCESS;
}

int create_db_header(int fd, struct db_header_t **headerOut) {

  struct db_header_t *header = calloc(1, sizeof(struct db_header_t));
  if (header == NULL) {
    printf("failed to calloc new header\n");
    perror("calloc");
    return STATUS_ERROR;
  }

  header->magic = HEADER_MAGIC;
  header->version = VERSION;
  header->count = 0;
  header->filesize = sizeof(struct db_header_t);

  *headerOut = header;

  return STATUS_SUCCESS;
}

int validate_db_header(int fd, struct db_header_t **headerOut) {

  // read
  if (fd < 0) {
    printf("invalid file descriptor\n");
    return STATUS_ERROR;
  }

  struct db_header_t *header = calloc(1, sizeof(struct db_header_t));
  if (header == NULL) {
    printf("failed to calloc header for validation\n");
    perror("calloc");
    return STATUS_ERROR;
  }

  if (read(fd, header, sizeof(struct db_header_t)) != sizeof(struct db_header_t)) {
    perror("read");
    free(header);
    return STATUS_ERROR;
  }

  // unpack
  header->magic = ntohl(header->magic);
  header->version = ntohs(header->version);
  header->count = ntohs(header->count);
  header->filesize = ntohl(header->filesize);

  // validate
  if (header->magic != HEADER_MAGIC) {
    printf("not a valid file format\n");
    free(header);
    return STATUS_ERROR;
  }

  if (header->version != VERSION) {
    printf("incorrect version, this version of dbview is only compatable with version %d\n", VERSION);
    free(header);
    return STATUS_ERROR;
  }

  struct stat fileStat = {0};
  if (fstat(fd, &fileStat) < 0) {
    perror("stat");
    close(fd);
    return STATUS_ERROR;
  }

  if (header->filesize != fileStat.st_size) {
    printf("filesize does not match expected size\n");
    free(header);
    return STATUS_ERROR;
  }

  *headerOut = header;

  return STATUS_SUCCESS;
}

int read_employees(int fd, struct db_header_t *header, struct employee_t **employeesOut) {

  if (fd < 0) {
    printf("invalid file descriptor\n");
    return STATUS_ERROR;
  }

  int count = header->count;

  struct employee_t *employees = calloc(count, sizeof(struct employee_t));
  if (employees == NULL) {
    printf("failed to calloc header for validation\n");
    perror("calloc");
    return STATUS_ERROR;
  }

  //cursor is in correct place from validate_db_header
  if (read(fd, employees, count * sizeof(struct employee_t)) != count * sizeof(struct employee_t)) {
    perror("read");
    free(employees);
    return STATUS_ERROR;
  }

  for (int i = 0; i < count; i++) {
    employees[i].hours = ntohs(employees[i].hours);
  }

  *employeesOut = employees;

  return STATUS_SUCCESS;
}

int add_employee(struct db_header_t *header, struct employee_t *employees, char *addStr){

  printf("%s\n", addStr);

  char *name = strtok(addStr, ",");
  char *address = strtok(NULL, ",");
  char *hours = strtok(NULL, ",");

  strncpy(employees[header->count - 1].name, name, sizeof(employees[header->count - 1].name));
  strncpy(employees[header->count - 1].address, address, sizeof(employees[header->count - 1].address));
  employees[header->count - 1].hours = atoi(hours);

  return STATUS_SUCCESS;
}

void list_employees(struct db_header_t *header, struct employee_t *employees){

  for (int i=0; i < header->count; i++) {
    printf("Employee %d\n", i);
    printf("\tName: %s\n", employees[i].name);
    printf("\tAddress: %s\n", employees[i].address);
    printf("\tHours: %d\n", employees[i].hours);
  }
}
int remove_employee(struct db_header_t *header, struct employee_t *employees, char *todelete){

  int deleted = 0;
  for (int i=0; i<header->count; i++) {
    if (strcmp(employees[i].name, "todelete") == 0) {
      deleted++;
      memmove(&employees[i], &employees[i + 1], header->count - i - 1 * sizeof(struct employee_t));
    }
  }
  return deleted;
}
