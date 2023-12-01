#ifndef __COMMANDS_H__
#define __COMMANDS_H__

#include <stdio.h>
#include <string.h>
#include <limits.h>
#include <sys/ioctl.h>
#include "eddata.h"
#include "fdctrl.h"
#include "ttyctrl.h"
#include "conversion.h"

/*
 * Clears the screen using ANSI clear
 */
void cmdclear(struct eddata *, const char **);

/*
 * Writes changes to the file
 */
void cmdwrite(struct eddata *, const char **);

/*
 * Edits a line in the file
 */
void cmdedit(struct eddata *, const char **);

/*
 * Prints information about the file to the screen
 */
void cmdinfo(struct eddata *, const char **);

/*
 * Prints out the first ten lines of a file
 */
void cmdhead(struct eddata *, const char **);

/*
 * Prints out a range of lines, say 1 - 10
 */
void cmdrange(struct eddata *, const char **);

void cmdfill(struct eddata *, const char **);

/*
 * Tries to convert string to integer and print the line
 */
void cmdprint(struct eddata *, const char **);

struct commandmap {
	const char * cmd;
	void (*fn) (struct eddata *, const char **);
};

/*
 * Checks that a string is a valid command,
 * If it is then it will call the function
 */
signed int iscommand(struct eddata *, const char **);

#endif /* __COMMANDS_H__ */
