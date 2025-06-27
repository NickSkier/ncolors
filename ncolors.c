#include <stdlib.h>
#include <ncurses.h>

int main(int argc, char *argv[])
{
	initscr();
	if(has_colors() == FALSE)
	{
		endwin();
		printf("Your terminal does not support color\n");
		exit(1);
	}
	start_color();

	int color = 0;
	int cell_size = 5;
	for (int i = 1; i <= 16; ++i)
		for (int j = 1; j <= 16; ++j) {
			if (color++ == 256) break;
			init_pair(color, COLOR_BLACK, color);
			attron(COLOR_PAIR(color));
			mvprintw(j-1, i*cell_size-cell_size, "%*d", cell_size, color);
			// attroff(COLOR_PAIR(color));
		}

	getch();
	endwin();
}
