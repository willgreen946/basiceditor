#include <stdio.h>
#include <fcntl.h>
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "fdctrl.h"

/*
 * Return size of file in bytes
 */
off_t
fdgetsize(int fd)
{
	off_t size;

	/*
   * Get the size of the file
	 */
	if ((size = lseek(fd, (size_t) 0, SEEK_END)) < 0) {
		fprintf(stderr,
			"ERROR: %s : %s\n",
			__func__, strerror(errno));
		return (off_t) -1;
	}

	/*
	 * Set the position to the start
	 */
	if (lseek(fd, (size_t) 0, SEEK_SET) < 0) {
		fprintf(stderr,
			"ERROR: %s : %s\n",
			__func__, strerror(errno));
		return (off_t) -1;
	}

	return size;
}

char **
fdreadfile(int fd, off_t fsize, const char * mode)
{
	size_t i;
	size_t buflen;
	FILE * fp = (FILE *) 0;
	char * buf = (char *) 0;
	char ** linev = (char **) 0;

	/*
	 * Allocate memory for the line vector
	 */
	if (!(linev = (char **) malloc((size_t) fsize))) {
		fprintf(stderr,
			"ERROR: %s : %s\n",
			__func__, "bad alloc");
		return (char **) 0;
	}

	/*
	 * Allocate memory for buffer
	 */
	if (!(buf = (char *) malloc((size_t) fsize))) {
		fprintf(stderr,
			"ERROR: %s : %s\n",
			__func__, "bad alloc");
		return (char **) 0;
	}

	/*
	 * Create file pointer
	 */
	if (!(fp = fdopen(fd, mode))) {
		fprintf(stderr,
			"ERROR: %s : %s\n",
			__func__, strerror(errno));
		return (char **) 0;
	}

	/*
	 * Read from the file pointer
	 */
	for (i = 0; fgets(buf, (int) fsize, fp); i++) {
		if (!buf)
			return (char **) 0;

		buflen = strnlen(buf, (size_t) fsize);

		if (!(linev[i] = strndup(buf, buflen))) {
			fprintf(stderr,
				"ERROR: %s : %s\n",
				__func__, strerror(errno));
			return (char **) 0;
		}

		memset(buf, 0, buflen);
	}

	/*
	 * Destroy file pointer
	 */
	if (fclose(fp) < 0) {
		fprintf(stderr,
			"ERROR: %s : %s\n",
			__func__, strerror(errno));
		return (char **) 0;
	}	

	if (buf)
		free(buf);

	return linev;
}
