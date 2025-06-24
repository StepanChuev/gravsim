#ifndef IO_H
#define IO_H

#include "body.h"

#ifdef _WIN32
#include <conio.h>
#else
#define clrscr() printf("\e[1;1H\e[2J")
#endif

typedef struct Flags {
	char ispause;
	char isdraw;
	int width;
	int height;
	float scale_step;
	float move_step;
	size_t fix_i;
	char *sys_filepath;
	char *save_dir;
	char *save_pref;
} Flags;

Flags *get_flags_from_args(int argc, char **argv, Flags *default_flags);
void log_bodies(Body *bodies, size_t len);

#endif