#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>

typedef enum {
	ALL_COLORS,
} OutputType;

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

int main(int argc, char *argv[])
{
	OutputType output;
	if (argc == 1) output = ALL_COLORS;
	else {
		printf("Wrong number of arguments\n");
		exit(1);
	}

	if (output == ALL_COLORS) {
		init_ncurses();
		print_all_colors();
	}
	else {
		printf("Wrong arguments values\n");
		exit(1);
	}

	getch();
	endwin();
}
