# Everything else
CC = gcc

# OpenBSD
#CC = egcc

WARNINGS = -Wall -Wextra -Wshadow -Wpointer-arith \
	-Wcast-align -Wwrite-strings -Wmissing-prototypes \
	-Wmissing-declarations -Wredundant-decls -Wnested-externs \
	-Winline -Wuninitialized -Wconversion -Wstrict-prototypes \
	-Wpedantic -pedantic -ansi

# OpenBSD & FreeBSD
#INCS = -I/usr/local/include
#LIBS = -L/usr/local/lib

# NetBSD
INCS = -I/usr/include
LIBS = -L/usr/lib

CFLAGS = $(INCS) $(LIBS) $(WARNINGS) \
	-std=c99 -lc -O0 -g

all:
	$(CC) $(CFLAGS) src/main.c -c
	$(CC) $(CFLAGS) src/filemap.c -c
	$(CC) $(CFLAGS) src/fdctrl.c -c
	$(CC) $(CFLAGS) main.o fdctrl.o filemap.o
