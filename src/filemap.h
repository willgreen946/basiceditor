#ifndef __FILEMAP_H__
#define __FILEMAP_H__

#include <errno.h>
#include <stdio.h>
#include <fcntl.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

struct filemap;

int filemapinit(struct filemap *, const char *);

int filemapdestroy(struct filemap *);

#endif /* __FILEMAP_H__ */
