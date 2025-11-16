#include <errno.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>
#include <string.h>

typedef enum {
	ALL_COLORS,
	CUSTOM_COLOR_RGB,
	CUSTOM_COLOR_NCURSES_RGB,
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

bool parse_rgb(const char *arg_r, const char *arg_g, const char *arg_b
               , short *r, short *g, short *b
               , bool ncurses_rgb
              ) {
	if (!parse_short(arg_r, r) ||
			!parse_short(arg_g, g) ||
			!parse_short(arg_b, b)
		 ) {
		return false;
	}
	if ((ncurses_rgb && (*r > 1000 || *g > 1000 || *b > 1000))
			|| (!ncurses_rgb && (*r > 255 || *g > 255 || *b > 255))) {
		return false;
 	}
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

void print_rgb_color(short r, short g, short b, bool ncurses_rgb) {
	if (ncurses_rgb)
		init_color(1, r, g, b);
	else
		init_color(1, (short)(r*1000/255), (short)(g*1000/255), (short)(b*1000/255));

	init_pair(1, 1, 1);
	attron(COLOR_PAIR(1));
	short cell_size = 5;
	printw("%*c", cell_size, '.');
	attroff(COLOR_PAIR(1));

	if (ncurses_rgb) {
		printw(" rgb(%d, %d, %d)\n", r*255/1000, g*255/1000, b*255/1000);
		printw("ncurses values rgb(%d, %d, %d)", r, g, b);
	}
	else {
		printw(" rgb(%d, %d, %d)\n", r, g, b);
		printw("ncurses values rgb(%d, %d, %d)"
		       , (short)(r*1000/255), (short)(g*1000/255), (short)(b*1000/255)
		      );
	}
}

void print_help(const char *bin_path) {
	printf("ncolors: display default 255 ncurses colors or display custom rgb color.\n");
	printf("Usage: %s FLAG [VALUES]\n", bin_path);
	printf("\n");
	printf("Options:\n");
	printf("  -h,--help\t\t\tPrint this help message and exit\n");
	printf("  -a,--all\t\t\tDisplay 255 default ncurses colors\n");
	printf("  --rgb VAL VAL VAL\t\tDisplay custom rgb color using values from 0 to 255\n");
	printf("  --ncurses-rgb VAL VAL VAL\tDisplay custom rgb color using values from 0 to 1000\n");
	printf("\n");
	printf("Example: ncolors --rgb 1 56 99\n");
	printf("To exit press any key\n");
}

int main(int argc, char *argv[])
{
	OutputType output;
	if (argc == 2 &&  (!strcmp(argv[1], "-a") || !strcmp(argv[1], "--all"))) {
		output = ALL_COLORS;
	}
	else if (argc == 2 && (!strcmp(argv[1], "-h") || !strcmp(argv[1], "--help"))) {
		print_help(argv[0]);
		exit(0);
	}
	else if (argc == 5 && !strcmp(argv[1], "--rgb")) {
		output = CUSTOM_COLOR_RGB;
	}
	else if (argc == 5 && !strcmp(argv[1], "--ncurses-rgb")) {
		output = CUSTOM_COLOR_NCURSES_RGB;
	}
	else if (argc == 1) {
		printf("Arguments requeired. Use -h,--help to print help message.\n");
		exit(1);
	}
	else {
		printf("Wrong arguments. Use -h,--help to print help message.\n");
		exit(1);
	}

	short r, g, b;
	if (output == ALL_COLORS) {
		init_ncurses();
		print_all_colors();
	}
	else if (output == CUSTOM_COLOR_RGB
					&& parse_rgb(argv[2], argv[3], argv[4], &r, &g, &b, false)
		      ) {
		init_ncurses();
		print_rgb_color(r, g, b, false);
	}
	else if (output == CUSTOM_COLOR_NCURSES_RGB
					&& parse_rgb(argv[2], argv[3], argv[4], &r, &g, &b, true)
		      ) {
		init_ncurses();
		print_rgb_color(r, g, b, true);
	}
	else {
		printf("Wrong arguments values. Use -h,--help to print help message.\n");
		exit(1);
	}

	getch();
	endwin();
	return 0;
}
