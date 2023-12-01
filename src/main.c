#include <stdio.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <signal.h>
#include "eddata.h"
#include "commands.h"

static int help(const char *);
static int setup(const char **);
static int scanargs(const char **);
static void signalhandler(int);
static int eventloop(struct eddata *);
static int parseline(struct eddata *, char *);

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
	struct eddata ed;

	if (scanargs(argv))
		return 1;

	/*
	 * Cancel ctrl + c and other signals
	 */
	signal(SIGINT, signalhandler);

	/*
	 * A loop for opening files
	 */
	while (*++argv) {
		if (**argv == '-')
			; /* Skip */
		else {
			if (eddatainit(&ed, *argv))
				return 1;

			if (eventloop(&ed))
				return 1;

			if (eddatadestroy(&ed))
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

static void
signalhandler(int sig)
{
	switch (sig) {
		case SIGINT:
		case SIGQUIT:
			fputs("'quit' to exit the editor\n", stderr);
			break;
	}
}

/*
 * Input loop
 */
static int
eventloop(struct eddata * ed)
{
	char buf[256];

	for (;;) {
		if (!fgets(buf, 255, stdin)) {
			fprintf(stderr,
				"ERROR: %s : %s\n",
				__func__, "fgets");
			return 1;
		}

		if (*buf && *buf != '\n') {
			if (!strncmp(buf, "quit", 4))
				return 0;

			if (parseline(ed, buf))
				return 1;
		}	
	}

	return 0;
}

static int 
parseline(struct eddata * ed, char * s)
{
	char ** p;
	char * argv[256];

	/* Remove newline char */
	s[strlen(s) - 1] = '\0';

	/*
	 * Split up string by whitespace
	 */
	for (p = argv; p < &argv[255] && 
		(*p = strsep(&s, " \t"));) {
		if (**p)
			p++;
	}

	if (iscommand(ed, (const char **) argv) < 0) 
		fputs("?\n", stderr);

	return 0;
}
