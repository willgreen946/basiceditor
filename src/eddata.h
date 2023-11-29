#ifndef __FILEMAP_H__
#define __FILEMAP_H__

#include <errno.h>
#include <stdio.h>
#include <fcntl.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

struct eddata {
	size_t linemax;
	off_t filesize;
	int filedescriptor;
	char ** linevector;
	const char * filename;
};

int eddatainit(struct eddata *, const char *);

int eddatadestroy(struct eddata *);

#endif /* __FILEMAP_H__ */
