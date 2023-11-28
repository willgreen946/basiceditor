#include <stdio.h>
#include <string.h>
#include <limits.h>
#include "filemap.h"
#include "commands.h"
#include "conversion.h"

struct commandmap cmdmap[COMMANDMAPMAX] = {
	{ "print", cmdprint },
	{ "range", cmdrange },
	{ "head", cmdhead },
	{ "info", cmdinfo },
};

signed int
cmdinfo(struct filemap * fm, const char ** argv)
{
	if (argv[1])
		fputs("Command info takes no arguments ...\n", stderr);

	fprintf(stdout,
		"%s\nsize - %lu\nlines - %lu\n",
		fm->filename, fm->filesize, fm->linemax);

	return 0;
}

/*
 * Prints first ten lines of a file
 */
signed int
cmdhead(struct filemap * fm, const char ** argv)
{
	const char * av[3] = { "range", "1", "10" };

	if (argv[1])
		fputs("Command head takes no arguments ...\n", stderr);

	return cmdrange(fm, (const char **) av);
}

/*
 * Prints out lines of a file from a range
 */
signed int
cmdrange(struct filemap * fm, const char ** argv)
{
	size_t i;
	size_t end = 0;
	size_t start = 0;

	if (!argv[1] || !argv[2]) {
		fputs("Expected two arguments\n", stderr);
		return 0;
	}

	if (!convsize_t(&start, argv[1]))
		return 0;

	if (!convsize_t(&end, argv[2]))
		return 0;

	if (!fm->linevector[end - 1]) {
		fprintf(stderr,
			"ERROR: %s : %lu no such line\n", __func__, end);
		return 0;
	}

	if (!fm->linevector[start - 1]) {
		fprintf(stderr,
			"ERROR: %s : %lu no such line\n", __func__, start);
		return 0;
	}

	for (i = (start - 1); i < end; i++) {
		if (fm->linevector[i])
			fprintf(stdout, "%lu: %s", i + 1, fm->linevector[i]);
	}

	return 0;
}

/*
 * Prints out a line from a file
 */
signed int
cmdprint(struct filemap * fm, const char ** argv)
{
	size_t ln = 0;

	while (*++argv) {
		if (!convsize_t(&ln, *argv))
			return 0;

		if (fm->linevector[ln - 1])
			fprintf(stdout, "%lu: %s", ln, fm->linevector[ln - 1]);

		else {
			fprintf(stderr,
				"ERROR: %s : %lu no such line\n", __func__, ln);
			return 0;
		}
	}

	return 0;
}

signed int
iscommand(struct filemap * fm, const char ** argv)
{
	size_t i;

	if (!argv || !argv[0])
		return -1;

	for (i = 0; i < COMMANDMAPMAX; i++) {
		if (!strncmp(cmdmap[i].cmd, argv[0], strlen(cmdmap[i].cmd)))
			return cmdmap[i].fn(fm, argv);
	}

	return -1;
}
