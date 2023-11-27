#include <errno.h>
#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "fdctrl.h"
#include "filemap.h"

int
filemapinit(struct filemap * fm, const char * file)
{
	/*
	 * Open the file for reading and writing
   */
	if ((fm->filedescriptor = open(file, O_RDWR)) < 0) {
		fprintf(stderr,
			"ERROR: %s : %s : %s\n",
			__func__, strerror(errno), file);
		return 1;
	}

	fm->filename = file;

	/*
	 * Get the size of the file
	 */
	if ((fm->filesize = fdgetsize(fm->filedescriptor)) < 0)
		return 1;

	/*
	 * Read the file into memory
	 */
	if (!(fm->linevector = fdreadfile(fm->filedescriptor)))
		return 1;

	return 0;
}

int
filemapdestroy(struct filemap * fm)
{
	size_t i;
	int retval = 0;

	/*
	 * Close the open file
	 */
	if (close(fm->filedescriptor) < 0) {
		fprintf(stderr,
			"ERROR: %s : %s : %s\n",
			__func__, strerror(errno), fm->filename);
		retval = 1;
	}

	/*
	 * Free memory in linevector
	 */
	if (fm->linevector)
		for (i = 0; fm->linevector[i]; i++) {
			free(fm->linevector[i]);

		free(fm->linevector);
	}

	fm->filesize = 0;
	fm->linemax = 0;

	return retval;
}
