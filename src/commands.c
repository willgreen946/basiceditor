#include <stdio.h>
#include <string.h>
#include <limits.h>
#include <sys/ioctl.h>
#include "eddata.h"
#include "fdctrl.h"
#include "ttyctrl.h"
#include "conversion.h"
#include "commands.h"

/*
 * Number of commands
 */
enum { COMMANDMAPMAX = 17 };

struct commandmap cmdmap[COMMANDMAPMAX] = {
	/* Full cmd names */
	{ "print", cmdprint },
	{ "range", cmdrange },
	{ "fill", cmdfill },
	{ "head", cmdhead },
	{ "info", cmdinfo },
	{ "edit", cmdedit },
	{ "write", cmdwrite },
	{ "clear", cmdclear },

	/* Shorthand cmds */
	{ "ed", cmdedit },
	{ "fi", cmdfill },
	{ "pr", cmdprint },
	{ "ra", cmdrange },
	{ "cls", cmdclear },
	{ "he", cmdhead },
	{ "wr", cmdwrite },
};

/*
 * Clears the screen using ANSI clear
 */
void
cmdclear(struct eddata * ed, const char ** argv)
{
	if (!argv || !ed)
		{} /* Silence warnings */

	fprintf(stdout, "\x1b[H\x1b[J");
}

/*
 * Writes data from ed to the file
 */
void
cmdwrite(struct eddata * ed, const char ** argv)
{
	size_t i;
	FILE * fp;

	if (argv[1])
		fputs("WARNING: write takes no arguments.\n", stderr);

	if (fdisfile(".bed.tmp")) {
		if (fdremovefile(".bed.tmp") < 0)
			return;
	}

	if (!(fp = fopen(".bed.tmp", "a+"))) {
		fprintf(stderr,
			"ERROR: %s : %s\n",
			__func__, strerror(errno));
		return;
	}

	for (i = 0; i < ed->linemax; i++) {
		if (ed->linevector[i])
			fputs(ed->linevector[i], fp);
		else
			break;
	}

	if (fclose(fp) < 0) {
		fprintf(stderr,
			"ERROR: %s : %s\n",
			__func__, strerror(errno));
		return;
	}
}

/*
 * Edits a line from the line map in ed
 */
void
cmdedit(struct eddata * ed, const char ** argv)
{
	size_t ln;
	char buf[1024];

	if (!argv[1]) {
		fputs(
			"ERROR: Edit requires a line number!\n",
			stderr);
		return;
	}

	if (!convsize_t(&ln, argv[1]))
		return;

	if (!(fgets(buf, sizeof(buf), stdin))) {
		fprintf(stderr,
			"ERROR: %s : fgets\n",
			__func__);
		return;
	}

	/*
	 * Clear the line if it exists
	 */
	if (ed->linevector[ln]) {
		memset(
			ed->linevector[ln],
			0, strlen(ed->linevector[ln]));
		free(ed->linevector[ln]);
	}

	if (!(ed->linevector[ln] = strndup(buf, strlen(buf))))
		fprintf(stderr,
			"ERROR: %s : bad alloc\n",
			__func__);
}

/*
 * Prints out information about the file
 */
void
cmdinfo(struct eddata * ed, const char ** argv)
{
	if (argv[1])
		fputs("WARNING: info takes no arguments\n", stderr);

	fprintf(stdout,
		"%s : File\n%lu : Size\n%lu : Lines\n",
		ed->filename, ed->filesize, ed->linemax);
}

/*
 * Prints first ten lines of a file
 */
void
cmdhead(struct eddata * ed, const char ** argv)
{
	const char * av[3] = { "range", "1", "10" };

	if (argv[1])
		fputs("Command head takes no arguments ...\n", stderr);

	cmdrange(ed, (const char **) av);
}

/*
 * Prints out lines of a file from a range
 */
void
cmdrange(struct eddata * ed, const char ** argv)
{
	size_t i;
	size_t end = 0;
	size_t start = 0;

	if (!argv[1] || !argv[2]) {
		fputs("Expected two arguments\n", stderr);
		return;
	}

	/*
	 * Converts the strings into numbers
	 */
	if (!convsize_t(&start, argv[1]))
		return;

	if (!convsize_t(&end, argv[2]))
		return;

	if (!ed->linevector[end - 1]) {
		fprintf(stderr,
			"ERROR: %s : %lu no such line!\n", __func__, end);
		return;
	}

	if (!ed->linevector[start - 1]) {
		fprintf(stderr,
			"ERROR: %s : %lu no such line!\n", __func__, start);
		return;
	}

	if (end < start) {
		fprintf(stderr,
			"ERROR: %s : End interger is greater than start!\n",
			__func__);
		return;
	}

	/*
	 * Start printing the lines
	 */
	for (i = (start - 1); i < end; i++) {
		if (ed->linevector[i])
			fprintf(stdout, "%lu: %s", i + 1, ed->linevector[i]);
	}
}

void
cmdfill(struct eddata * ed, const char ** argv)
{
	size_t i;
	size_t ln;

	/*
	 * Get the size of the terminal
	 */
	ttygetsize(&ed->ts);

	if (!argv[1]) {
		fprintf(stderr,
			"ERROR: %s : %s\n",
			__func__, "Fill needs one argument");
		return;
	}

	/* Convert string to size_t */
	if (!convsize_t(&ln, argv[1]))
		return;

	/*
	 * Make sure starting line exists
	 */
	if (!(ed->linevector[ln - 1])) {
		fprintf(stderr,
			"ERROR: %s : %lu No such line\n",
			__func__, ln);
		return;
	}

	/*
	 * Start printing out lines
	 */
	for (i = ln - 1;
		i < ((ln - 1) + (size_t) ed->ts.rows); i++) {
		if (ed->linevector[i])
			fprintf(stdout,
				"%lu: %s",
				i, ed->linevector[i]);
		else return;
	}
}

/*
 * Prints out a line from a file
 */
void
cmdprint(struct eddata * ed, const char ** argv)
{
	size_t ln = 0;

	while (*++argv) {
		if (!convsize_t(&ln, *argv))
			return;

		if (ed->linevector[ln - 1])
			fprintf(stdout, "%lu: %s", ln, ed->linevector[ln - 1]);

		else {
			fprintf(stderr,
				"ERROR: %s : %lu no such line\n", __func__, ln);
			return;
		}
	}
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
		if (!strncmp(cmdmap[i].cmd, argv[0], strlen(cmdmap[i].cmd))) {
			cmdmap[i].fn(ed, argv);
			return 1;
		}
	}

	return -1;
}
