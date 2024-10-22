#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>

#include "../include/common.h"
#include "../include/file.h"

int create_db_file(char* filepath) {

  int check_fd = open(filepath, O_RDONLY); 
  if (check_fd != -1) {
    close(check_fd);
    printf("file '%s' already exists\n", filepath);
    return STATUS_ERROR;
  }

  int fd = open(filepath, O_RDWR | O_CREAT, 0644);
  if (fd == -1) {
    perror("open");
    return STATUS_ERROR;
  }
  return fd;
}

int open_db_file(char* filepath) {

  int fd = open(filepath, O_RDWR); 
  if (fd == -1) {
    perror("open");
    return STATUS_ERROR;
  }
  return fd;
}
