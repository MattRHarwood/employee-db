#ifndef	PARSE_H
#define	PARSE_H

#define HEADER_MAGIC 0x4D484442
#define VERSION 1

struct db_header_t {
  unsigned int magic;
  unsigned short version;
  unsigned short count;
  unsigned int filesize;
};

struct employee_t {
  char name[256];
  char address[256];
  unsigned int hours;
};

int create_db_header(int fd, struct db_header_t **headerOut);
int validate_db_header(int fd, struct db_header_t **headerOut);
int read_employees(int fd, struct db_header_t *headerOut, struct employee_t **employeesOut);
int output_db_file(int fd, struct db_header_t *headerOut, struct employee_t *employeesOut);

#endif
