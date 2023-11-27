#ifndef __FDCTRL_H__
#define __FDCTRL_H__

#include <stdio.h>
#include <fcntl.h>
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

/*
 * Returns size of a file in bytes from a file descriptor
 * Returns -1 on failure, and prints out an error message
 */
off_t fdgetsize(int);

/*
 * Reads in a file into memory line by line
 */
char ** fdreadfile(int);

#endif /* __FDCTRL_H__ */
