/*****************************************************************************
 * Copyright (C) Snehal Tiwari tiwarism14.comp@coep.ac.in
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation; either version 2.1 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program; if not, write to the Free Software Foundation,
 * Inc., 51 Franklin Street, Fifth Floor, Boston MA 02110-1301, USA.
 *****************************************************************************/

#include <ncurses.h>
#include <unistd.h>
#include <string.h>
#include "pacman.h"
#include "render.h"
#include "game.h"
#define PC_PACMAN	1
#define PC_WALL	2
#define PC_PILL	3
#define PC_POWERPILL	4
#define PC_WHITE	5	
#define PC_EDIBLE_GHOST	6
#define PC_GHOST	20
void *InitRender(void) {
	int gcol[] = { COLOR_RED, COLOR_GREEN, COLOR_MAGENTA, COLOR_CYAN, };
	int i;
	initscr();
	cbreak();
	noecho();
	nodelay(stdscr, TRUE);
	curs_set(0);
	if (has_colors()) {
		fprintf(stderr, "Color console in use\n");
		start_color();
		init_pair(PC_PACMAN, COLOR_YELLOW, COLOR_BLACK);
		init_pair(PC_WALL, COLOR_BLUE, COLOR_BLACK);
		init_pair(PC_EDIBLE_GHOST, COLOR_MAGENTA, COLOR_BLACK);
		init_pair(PC_PILL, COLOR_CYAN, COLOR_BLACK);
		init_pair(PC_POWERPILL, COLOR_WHITE, COLOR_BLACK);
		init_pair(PC_WHITE, COLOR_WHITE, COLOR_BLACK);
		if (can_change_color()) {
			init_color(COLOR_GREEN, 1000,644,888);
			init_color(COLOR_MAGENTA, 1000,888,970);
			init_color(COLOR_CYAN, 1000,644,285);
		}
		else {
			fprintf(stderr, "Ghosts colors will not be accurate because TTY doesn't support init_color\n");
		}
		for(i=0; i<MAX_GHOSTS; i++)
			init_pair(PC_GHOST+i, gcol[i%sizeof(gcol)], COLOR_BLACK);
	}
	else {
		fprintf(stderr, "Black and white console in use\n");
	}
	return NULL;
}
int CloseRender(void *pCtx) {
	curs_set(1);
	endwin();
	return 1;
}
void Blit(void *pCtx) {
	refresh();
}
static int CenteredX(char *pStr) {
	int cx, leftedge=32;
	cx = COLS-leftedge;
	cx -= strlen(pStr);
	cx /= 2;
	cx += leftedge;
	if (cx<0) cx=32;
		return cx;
}
static void DrawSquare(void *pCtx, state *ptr, int x, int y) {
	char c;
	int col;
	GetMap(ptr, x,y, &c);
	switch(c) {
		case	'#': col=PC_WALL; 
		break;
		case	'.': col=PC_PILL; 
		break;
		case	'$':
		case	'P':
		case	'H': c=' ';
		default: col=PC_WHITE; 
		break;
	}
	attron(COLOR_PAIR(col));
	mvaddch(y,x, c);
	attroff(COLOR_PAIR(col));
}
int DrawBoard(void *pCtx, state *ptr) {
	int x, y;
	for(y = 0; y < ptr->iMapHeight; y++)
		for(x = 0 ; x < ptr->iMapWidth; x++)
			DrawSquare(pCtx, ptr, x, y);
	return 0;
}
int DrawPlayer(void *pCtx, state *ptr) {
	static char *szAnimation[] = { 
	/*Left*/	"})>->)",
	/*Right*/	"{(<-<(",
	/*Up*/	"VVVV||",
	/*Down*/	"^^^^||",
	};
	int c;
	switch(ptr->Player.iLastValidDir) {
		case	PACKEY_LEFT: c=0; 
		break;
		case	PACKEY_RIGHT: c=1; 
		break;
		case	PACKEY_UP: c=2; 
		break;
		case	PACKEY_DOWN: c=3; 
		break;
	}
	attron(A_BOLD);
	attron(COLOR_PAIR(PC_PACMAN));
	mvaddch(ptr->Player.Pos.y, ptr->Player.Pos.x, szAnimation[c][ptr->Player.iAnimFrame]);
	attroff(COLOR_PAIR(PC_PACMAN));
	attroff(A_BOLD);
	return 0;
}
int DrawGhosts(void *pCtx, state *ptr) {
	ghost *pG = &ptr->Ghosts[0];
	int i;
	for(i = 0; i < MAX_GHOSTS; i++, pG++)
		if (pG->bActive) {
			int col = COLOR_PAIR(PC_GHOST+i);
			if (pG->fEdibleDuration) {
				if (pG->fEdibleDuration < 2)
					attron(A_BLINK);
				col = COLOR_PAIR(PC_EDIBLE_GHOST);
			}
			attron(col);
			mvaddch(pG->Pos.y, pG->Pos.x, 'M');
			attroff(col);
			if (pG->fEdibleDuration && pG->fEdibleDuration < 2)
				attroff(A_BLINK);
		}
	return 0;
}
int DrawPills(void *pCtx, state *ptr) {
	char szSpecial[] = {'$','@','%','!',};
	int i;
	attron(A_BLINK);
	attron(COLOR_PAIR(PC_POWERPILL));
	for(i = 0;i < MAX_PILLS; i++)
		if (!ptr->Pills[i].bEaten)
			mvaddch(ptr->Pills[i].Pos.y, ptr->Pills[i].Pos.x, '*');
	attroff(COLOR_PAIR(PC_POWERPILL));
	attroff(A_BLINK);
	if (ptr->fSpecialDuration) {
		attron(A_BOLD);
	mvaddch(ptr->SpecialPos.y,ptr->SpecialPos.x,szSpecial[ptr->iSpecialChar]);
	attroff(A_BOLD);
	}
	return 0;
}
void FlashBoard(void *pCtx, state *ptr) {
	int x, y;
	int cnt=9;
	while(cnt--) {
		if (cnt&1)
			attron(A_BOLD);
		for(y = 0;y < ptr->iMapHeight; y++)
			for(x = 0; x<ptr->iMapWidth; x++)
				DrawSquare(pCtx, ptr, x, y);
		if (cnt&1) 
			attroff(A_BOLD);
		refresh();
		usleep(300000);
	}
}
int DrawEndGame(void *pCtx, state *ptr) {
	attron(A_BOLD | A_BLINK);
	mvprintw(14, 9, "GAME OVER");
	attroff(A_BOLD | A_BLINK);
	Blit(pCtx);
	return 0;
}
void AnimateDeadPlayer(void *pCtx, state *ptr) {
	int iFrame = 0;
	char *szAnim = "|Vv_.+*X*+. ";
	do {
		DrawBoard(pCtx, ptr);
		DrawPills(pCtx, ptr);
		attron(A_BOLD | COLOR_PAIR(PC_PACMAN));
		mvaddch(ptr->Player.Pos.y, ptr->Player.Pos.x, szAnim[iFrame]);
		attroff(A_BOLD | COLOR_PAIR(PC_PACMAN));
		Blit(pCtx);
		usleep(150000);
	} while(++iFrame < strlen(szAnim));
}
void RenderGameInfo(void *pCtx) {
	char *s1 = "Pacman - Snehal Tiwari";
	char *s2 = "COEP";
	clear();
	attron(COLOR_PAIR(PC_PILL));
	mvprintw(2,CenteredX(s1), s1);
	attroff(COLOR_PAIR(PC_PILL));
	attron(COLOR_PAIR(PC_PACMAN));
	mvprintw(3,CenteredX(s2), s2);
	attroff(COLOR_PAIR(PC_PACMAN));
}
void RenderRHS(void *pCtx, state *ptr) {
	char *pacimg[] = {"/-\\", "|'<", "\\_/", " ", " "," ", };
	int i,j;
	attron(A_BOLD | COLOR_PAIR(PC_PACMAN));
	for(i = 0; i<MAX_PACMAN_LIVES; i++)
		for(j=0; j<3; j++)
			mvprintw(16+j,i*5+30,pacimg[j+(ptr->Player.iLives>i?0:3)]);
	attroff(A_BOLD | COLOR_PAIR(PC_PACMAN));
	i = CenteredX("Score : 123456"); 
	mvprintw(5,i,"Score : %d", ptr->Player.iScore);
	mvprintw(6,i,"High : %d", ptr->iHighScore);
	mvprintw(8,i,"Level : %d", ptr->iLevel+1);
	if (LINES-1 >= ptr->iMapHeight) 
		i = 0;
	else
		i = ptr->iMapWidth;
	if (ptr->pMarquee)
		mvprintw(LINES-1, i, "%.*s", COLS-i, ptr->pMarquee);
}
int AnotherGame(void *pCtx) {
	char *s1 = "Another game?";
	char *s2 = "Y/N";
	attron(COLOR_PAIR(PC_GHOST));
	mvprintw(12,CenteredX(s1), s1);
	mvprintw(14,CenteredX(s2), s2);
	attroff(COLOR_PAIR(PC_GHOST));
	do {
		char ch = getch();
		if (ch=='y'||ch=='Y') 
			return 1;
		else if (ch=='n'||ch=='N') 
			return 0;
	} while(1);
	return 0;
}
