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
  char name[64];
  char address[64];
  unsigned int hours;
};

int create_db_header(int fd, struct db_header_t **headerOut);
int validate_db_header(int fd, struct db_header_t **headerOut);
int read_employees(int fd, struct db_header_t *header, struct employee_t **employeesOut);
int add_employee(int fd, struct db_header_t *header, struct employee_t *employees, char *addStr);
int output_db_file(int fd, struct db_header_t *header, struct employee_t *employees);
void list_employees(struct db_header_t *header, struct employee_t *employees);

#endif
