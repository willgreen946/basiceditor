#ifndef __COMMANDS_H__
#define __COMMANDS_H__

#include <stdio.h>
#include <string.h>
#include <limits.h>
#include "filemap.h"

enum { COMMANDMAPMAX = 4 };

signed int cmdinfo(struct filemap * fm, const char **);

signed int cmdhead(struct filemap * fm, const char **);

signed int cmdrange(struct filemap * fm, const char **);

/*
 * Tries to convert string to integer and print the line
 */
signed int cmdprint(struct filemap * fm, const char **);

struct commandmap {
	const char * cmd;
	signed int (*fn) (struct filemap *, const char **);
};

signed int iscommand(struct filemap *, const char **);

#endif /* __COMMANDS_H__ */
