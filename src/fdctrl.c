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
fdreadfile(int fd)
{
	char c;
	size_t i;
	off_t size;
	size_t linelen;
	ssize_t rvread;
	char * buf = (char *) 0;
	char ** lines = (char **) 0;

	if ((size = fdgetsize(fd)) < 0)
		return (char **) 0;

	/*
	 * Allocate the memory for the buffer
	 */
	if (!(buf = (char *) malloc((size_t) size))) {
		fprintf(stderr,
			"ERROR: %s : %s\n",
			__func__, "bad alloc");
		return (char **) 0;
	}

	memset(buf, 0, (size_t) size);

	/*
	 * Read file char by char
	 */
	for (i = 0; (rvread = read(fd, &c, 1));) {
		if (rvread < 0) {
			fprintf(stderr,
				"ERROR: %s : %s\n",
				__func__, strerror(errno));
			return (char **) 0;
		}

		/*
		 * Allocate memory for *line[]
		 */
		if (!lines) {
			if (!(lines = (char **) malloc((size_t) size))) {
				fprintf(stderr,
					"ERROR: %s : %s\n",
					__func__, "bad alloc");
				return (char **) 0;
			}
		}

		/*
		 * '\n' == EOL
		 */
		if (c == '\n') {
			strncat(buf, &c, 1);

			linelen = strnlen(buf, (size_t) size);

			if (!(lines[i++] = strndup(buf, linelen))) {
				fprintf(stderr,
					"ERROR: %s : %s\n",
					__func__, strerror(errno));
				return (char **) 0;
			}

			/*
			 * Stops malloc from allocating gigabytes
			 */
			memset(buf, 0, (size_t) size);
		} else
			strncat(buf, &c, 1);
	}

	if (buf)
		free(buf);

	return lines;
}
