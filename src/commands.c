#include <stdio.h>
#include <string.h>
#include <limits.h>
#include "filemap.h"
#include "commands.h"

struct commandmap cmdmap[] = {
	{ "print", cmdprint },
};

signed int
cmdprint(struct filemap * fm, const char ** argv)
{
	size_t ln;

	while (*++argv) {
		if (strspn(*argv, "+-.1234567890") != strlen(*argv)) {
			fprintf(stderr,
				"ERROR: %s : is not numeric\n",
				*argv);
			return 0;
		}

		ln = (size_t) strtol(*argv, (char **) 0, 10) - 1;

		if (fm->linevector[ln])
			fputs(fm->linevector[ln], stdout);

		else {
			fprintf(stderr, "ERROR: %lu : No such line\n", ln + 1);
			return 0;
		}
	}

	return 0;
}

signed int
iscommand(struct filemap * fm, const char ** argv)
{
	size_t i;

	if (!argv)
		return -1;

	for (i = 0; cmdmap[i].cmd; i++) {
		if (!strncmp(cmdmap[i].cmd, argv[0], strlen(cmdmap[i].cmd)))
			return cmdmap[i].fn(fm, argv);
	}

	return -1;
}
