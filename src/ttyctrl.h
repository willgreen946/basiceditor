#ifndef __TTYCTRL_H__
#define __TTYCTRL_H__

#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <termios.h>
#include <sys/ioctl.h>

struct ttystat {
	int rows;
	int cols;
	struct termios origttysettings;
};

void ttyinitraw(struct ttystat *);

void ttyreset(struct ttystat *);

/*
 * Gets the size of the current terminal
 */
void ttygetsize(struct ttystat *);

#endif /* __TTYCTRL_H__ */
