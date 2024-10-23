#include <stdio.h>
#include <unistd.h>
#include <sys/stat.h>
#include <stdlib.h>

#include "../include/common.h"
#include "../include/parse.h"

int output_db_file(int fd, struct db_header_t *headerOut) {
  if (fd < 0) {
    printf("invalid file descriptor to output\n");
    return STATUS_ERROR;
  }

  headerOut->magic = htonl(headerOut->magic);
  headerOut->version = htons(headerOut->version);
  headerOut->count = htons(headerOut->count);
  headerOut->filesize = htonl(headerOut->filesize);

  lseek(fd, 0, SEEK_SET);

  write(fd, headerOut, sizeof(struct db_header_t));

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

int read_employees(int fd, struct db_header_t *headerOut, struct employee_t **employeesOut) {
  if (fd < 0) {
    printf("invalid file descriptor\n");
    return STATUS_ERROR;
  }

  int count = headerOut->count;

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
