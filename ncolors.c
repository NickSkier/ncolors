#include <errno.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>

typedef enum {
	ALL_COLORS,
	CUSTOM_COLOR_RGB
} OutputType;

bool parse_short(const char *str, short *result) {
	char *endptr;
	errno = 0;
	long val = strtol(str, &endptr, 10);
	if ((errno == ERANGE) || endptr == str || *endptr != '\0') {
		return false;
	}
	if (val > SHRT_MAX || val < SHRT_MIN) {
		return false;
	}
	*result = (short)val;
	return true;
}

void init_ncurses() {
	initscr();
	if(has_colors() == FALSE)
	{
		endwin();
		printf("Your terminal does not support color\n");
		exit(1);
	}
	start_color();
	use_default_colors();
}

void print_all_colors() {
	short color = 0;
	short cell_size = 5;
	for (short i = 1; i <= 16; ++i)
		for (short j = 1; j <= 16; ++j) {
			if (color++ == 256) break;
			init_pair(color, COLOR_BLACK, color);
			attron(COLOR_PAIR(color));
			mvprintw(j-1, i*cell_size-cell_size, "%*d", cell_size, color);
		}
	attroff(COLOR_PAIR(color));
}

void print_rgb_color(short r, short g, short b) {
	init_color(1, r, g, b);
	init_pair(1, 1, 1);
	attron(COLOR_PAIR(1));
	short cell_size = 5;
	printw("%*c", cell_size, '.');
	attroff(COLOR_PAIR(1));
	printw(" rgb(%d, %d, %d)\n", r*255/1000, g*255/1000, b*255/1000);
	printw("ncurses values rgb(%d, %d, %d)", r, g, b);
}

int main(int argc, char *argv[])
{
	OutputType output;
	if (argc == 1) output = ALL_COLORS;
	else if (argc == 4) output = CUSTOM_COLOR_RGB;
	else {
		printf("Wrong number of arguments\n");
		exit(1);
	}

	short r, g, b;
	if (output == ALL_COLORS) {
		init_ncurses();
		print_all_colors();
	}
	else if (output == CUSTOM_COLOR_RGB  &&
			parse_short(argv[1], &r) &&
			parse_short(argv[2], &g) &&
			parse_short(argv[3], &b)
		 ) {
		init_ncurses();
		print_rgb_color(r, g, b);
	}
	else {
		printf("Wrong arguments values\n");
		exit(1);
	}

	getch();
	endwin();
}
