#include <stdio.h>
#include <string.h>
#include <limits.h>
#include "eddata.h"
#include "commands.h"
#include "conversion.h"

/*
 * Number of commands
 */
enum { COMMANDMAPMAX = 6 };

struct commandmap cmdmap[COMMANDMAPMAX] = {
	{ "print", cmdprint },
	{ "range", cmdrange },
	{ "head", cmdhead },
	{ "info", cmdinfo },
	{ "edit", cmdedit },
	{ "write", cmdwrite },
};

signed int
cmdwrite(struct eddata * ed, const char ** argv)
{
	if (argv[1])
		fputs("write takes no arguments\n", stderr);

	return 0;
}

signed int
cmdedit(struct eddata * ed, const char ** argv)
{
	if (!argv[1]) {
		fputs("Edit requires a line number ...\n", stderr);
		return 0;
	}

	
	return 0;
}

/*
 * Prints out information about the file
 */
signed int
cmdinfo(struct eddata * ed, const char ** argv)
{
	if (argv[1])
		fputs("Command info takes no arguments ...\n", stderr);

	fprintf(stdout,
		"%s : File\n%lu : Size\n%lu : Lines\n",
		ed->filename, ed->filesize, ed->linemax);

	return 0;
}

/*
 * Prints first ten lines of a file
 */
signed int
cmdhead(struct eddata * ed, const char ** argv)
{
	const char * av[3] = { "range", "1", "10" };

	if (argv[1])
		fputs("Command head takes no arguments ...\n", stderr);

	return cmdrange(ed, (const char **) av);
}

/*
 * Prints out lines of a file from a range
 */
signed int
cmdrange(struct eddata * ed, const char ** argv)
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

	if (!ed->linevector[end - 1]) {
		fprintf(stderr,
			"ERROR: %s : %lu no such line\n", __func__, end);
		return 0;
	}

	if (!ed->linevector[start - 1]) {
		fprintf(stderr,
			"ERROR: %s : %lu no such line\n", __func__, start);
		return 0;
	}

	for (i = (start - 1); i < end; i++) {
		if (ed->linevector[i])
			fprintf(stdout, "%lu: %s", i + 1, ed->linevector[i]);
	}

	return 0;
}

/*
 * Prints out a line from a file
 */
signed int
cmdprint(struct eddata * ed, const char ** argv)
{
	size_t ln = 0;

	while (*++argv) {
		if (!convsize_t(&ln, *argv))
			return 0;

		if (ed->linevector[ln - 1])
			fprintf(stdout, "%lu: %s", ln, ed->linevector[ln - 1]);

		else {
			fprintf(stderr,
				"ERROR: %s : %lu no such line\n", __func__, ln);
			return 0;
		}
	}

	return 0;
}

/*
 * Checks that a command is valid,
 * If so, run the right function
 */
signed int
iscommand(struct eddata * ed, const char ** argv)
{
	size_t i;

	if (!argv || !argv[0])
		return -1;

	for (i = 0; i < COMMANDMAPMAX; i++) {
		if (!strncmp(cmdmap[i].cmd, argv[0], strlen(cmdmap[i].cmd)))
			return cmdmap[i].fn(ed, argv);
	}

	return -1;
}
