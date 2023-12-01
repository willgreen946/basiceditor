#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <termios.h>
#include <sys/ioctl.h>
#include "ttyctrl.h"

void
ttyinitraw(struct ttystat * ts)
{
	struct termios raw;

	/*
	 * Store original terminal settings
	 */
	tcgetattr(STDIN_FILENO, &ts->origttysettings);

	raw = ts->origttysettings;

	/*
	 * Disable settings
	 */
	raw.c_lflag &= ~(ECHO);

	tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);
}

void
ttyreset(struct ttystat * ts)
{
	tcsetattr(STDIN_FILENO, TCSAFLUSH, &ts->origttysettings);
}

void
ttygetsize(struct ttystat * ts)
{
	struct winsize w;
	
	/*
	 * Try get the tty size
	 */
	if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &w) < 0) {
		fprintf(stderr,
			"ERROR: %s : %s\n",
			__func__, strerror(errno));
		return;
	}

	ts->rows = w.ws_row;
	ts->cols = w.ws_col;
}
