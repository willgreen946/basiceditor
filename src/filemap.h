#ifndef __FILEMAP_H__
#define __FILEMAP_H__

#include <errno.h>
#include <stdio.h>
#include <fcntl.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

struct filemap {
	size_t linemax;
	off_t filesize;
	int filedescriptor;
	char ** linevector;
	const char * filename;
};

int filemapinit(struct filemap *, const char *);

int filemapdestroy(struct filemap *);

#endif /* __FILEMAP_H__ */
