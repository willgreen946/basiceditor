#include <errno.h>
#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "fdctrl.h"
#include "eddata.h"

int
eddatainit(struct eddata * ed, const char * file)
{
	/*
	 * Open the file for reading and writing
   */
	if ((ed->filedescriptor = open(file, O_RDWR)) < 0) {
		fprintf(stderr,
			"ERROR: %s : %s : %s\n",
			__func__, strerror(errno), file);
		return 1;
	}

	ed->filename = file;

	/*
	 * Get the size of the file
	 */
	if ((ed->filesize = fdgetsize(ed->filedescriptor)) < 0)
		return 1;

	/*
	 * Read the file into memory
	 */
	if (!(ed->linevector = 
		fdreadfile(ed->filedescriptor, ed->filesize, &ed->linemax, "r+w")))
		return 1;

	return 0;
}

int
eddatadestroy(struct eddata * ed)
{
	size_t i;
	int retval = 0;

	/*
	 * Close the open file
	 */
	if (close(ed->filedescriptor) < 0) {
		fprintf(stderr,
			"ERROR: %s : %s : %s\n",
			__func__, strerror(errno), ed->filename);
		retval = 1;
	}

	/*
	 * Free memory in linevector
	 */
	if (ed->linevector) {
		for (i = 0; ed->linevector[i]; i++)
			free(ed->linevector[i]);

		if (ed->linevector)
			free(ed->linevector);
	}

	ed->filesize = 0;
	ed->linemax = 0;

	return retval;
}
