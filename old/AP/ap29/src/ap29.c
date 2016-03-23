#include <stdio.h>
#include <stdlib.h>
#include <curses.h>
#include <sys/time.h>

void help(void){
	printw("Help\n");
	printw("F1 - Vypise Help\n");
	printw("F2 - Program spusta meranie\n");
	printw("     Meranie sa spusti stlacenim klavesy\n");
	printw("     a konci stlacenim rovnakej klavesy.\n");
	printw("F10 - Koniec\n");
	printw("ESC - Koniec\n");
}

void timer(void){

	struct timeval s, e, diff;
	unsigned int c = 0, key = 0;

	key = getch();
	gettimeofday(&s, NULL);

	printw("%c ", key);

	while((c = getch()) != key){}

	gettimeofday(&e, NULL);
	timersub(&e, &s, &diff);

	printw("... %ld.%2ld s\n", diff.tv_sec, (int)(diff.tv_usec * 0.0001));
}

int main(void){

	unsigned int c = 0;

	initscr();
	raw();
	keypad(stdscr, TRUE);
	noecho();

	while ((c = getch()) != 27){
		if(c == KEY_F(1)){
			help();
		}
		else if(c == KEY_F(2)){
			clear();
			attron(A_BOLD);
			printw("Stlac klavesu pre zacanie merania\n");
			attroff(A_BOLD);
			timer();
		}
		else if((c == KEY_F(10)) || (c == 27)){
			break;
		}
		refresh();
	}
	endwin();

	return 0;
}
