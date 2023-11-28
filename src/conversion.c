#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <limits.h>
#include "conversion.h"

int
convsize_t(size_t * n, const char * s)
{
	if (strspn(s, "1234567890") != strlen(s)) {
		fprintf(stderr,
			"ERROR: %s : %s is not numeric\n",
			__func__, s);
		return 0;
	}

	*n = (size_t) strtol(s, (char **) 0, 10);

	return 1;
}
