#include <stdio.h>
#include <ctype.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <limits.h>
#include <unistd.h>
#include "filemap.h"
#include "commands.h"

static int help(const char *);
static int setup(const char **);
static int scanargs(const char **);
static int eventloop(struct filemap *);
static int parseline(struct filemap *, char *);

int
main(int argc, const char ** argv)
{
	return (argc < 2) ? help(*argv) : setup(argv);
}

/*
 * Simple help function
 */
static int
help(const char * name)
{
	fprintf(stderr,
		"%s help:\n",
		name);

	return 1;
}

static int
setup(const char ** argv)
{
	struct filemap fm;

	if (scanargs(argv))
		return 1;

	/*
	 * A loop for opening files
	 */
	while (*++argv) {
		if (**argv == '-')
			; /* Skip */
		else {
			if (filemapinit(&fm, *argv))
				return 1;

			if (eventloop(&fm))
				return 1;

			if (filemapdestroy(&fm))
				return 1;
		}
	}

	return 0;
}

/*
 * Scans the arguments passed by the shell
 */
static int
scanargs(const char ** argv)
{
	int i;
	int k;

	for (i = 1; argv[i]; i++) {
		if (argv[i][0] == '-' && argv[i][1]) {
			for (k = 1; argv[i][k]; k++) {
				switch (argv[i][k]) {
					case 's':
					case 'h':
						break;
					default:
						fprintf(stderr,
							"ERROR: %s : %s : no such flag %c\n",
							__FILE__, __func__, argv[i][k]);
						return 1;
				}
			}
		}
	}

	return 0;
}

/*
 * Input loop
 */
static int
eventloop(struct filemap * fm)
{
	char buf[1024];
	ssize_t rvread;

	for (;;) {
		write(STDOUT_FILENO, ">", 1);

		if ((rvread = read(STDIN_FILENO, buf, 1024)) < 0) {
			fprintf(stderr,
				"ERROR: %s : %s : %s\n",
				__FILE__, __func__, strerror(errno));
			return 1;
		}

		if (*buf) {
			if (!strncmp(buf, "quit", 4))
				return 0;

			if (parseline(fm, buf))
				return 2;
		}	
	}

	return 0;
}

static int 
parseline(struct filemap * fm, char * s)
{
	char ** p;
	char * argv[256];

	s[strlen(s) - 1] = '\0';

	/*
	 * Split up string by whitespace
	 */
	for (p = argv; p < &argv[255] && 
		(*p = strsep(&s, " \t"));) {
		if (**p)
			p++;
	}

	if (iscommand(fm, (const char **) argv) < 0) 
		fputs("No such command\n", stderr);

	return 0;
}
