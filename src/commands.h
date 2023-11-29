#ifndef __COMMANDS_H__
#define __COMMANDS_H__

#include <stdio.h>
#include <string.h>
#include <limits.h>
#include "eddata.h"

signed int cmdwrite(struct eddata *, const char **);

signed int cmdedit(struct eddata *, const char **);

/*
 * Prints information about the file to the screen
 */
signed int cmdinfo(struct eddata *, const char **);

/*
 * Prints out the first ten lines of a file
 */
signed int cmdhead(struct eddata *, const char **);

/*
 * Prints out a range of lines, say 1 - 10
 */
signed int cmdrange(struct eddata *, const char **);

/*
 * Tries to convert string to integer and print the line
 */
signed int cmdprint(struct eddata *, const char **);

struct commandmap {
	const char * cmd;
	signed int (*fn) (struct eddata *, const char **);
};

/*
 * Checks that a string is a valid command,
 * If it is then it will call the function
 */
signed int iscommand(struct eddata *, const char **);

#endif /* __COMMANDS_H__ */
