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

#include <string.h>
#include <ncurses.h>
#include <stdio.h>
#include <sys/time.h>
#include <unistd.h>
#include <time.h>
#include "pacman.h"
#include "game.h"
#include "player.h"
#include "lives.h"
#include "render.h"
char *g_pLevel1Map = {"\
############################\
#............##.............\
#.####......##.#####.####..\
.P#.##.#####.##.#####.####P.\
#...........................\
#.####.##.########.##.####.#\
#......##....##....##......#\
######.#..## ## #####.######\
#.## ##..# \
#.## ##....# ....# \
######..# . HHHH # ...######\
. # .... . ..... \
####..... # HHHH.. # ...######\
#.## ....... ##.# \
#.## $ ##.# \
####.#.## ........# ##.######\
#............##............#\
#.#...#.....##....#..##.####.#\
#P..##................##..P#\
###.##.#..#..#..##.##.##.###\
#......##....##....##......#\
#.##########.##.##########.#\
#..........................#\
############################"};
int InitialiseGame(state *ptr) {
	ptr->iLevel = 0;
	RepopulateBoard(ptr);
	InitialisePlayer(ptr);
	InitialiseGhosts(ptr);
	InitialisePills(ptr);
	return 0;
}
int ReinitialiseGame(state *ptr)
{
	RepopulateBoard(ptr);
	ReinitialisePlayer(ptr);
	InitialiseGhosts(ptr);
	InitialisePills(ptr);
	return 0;
}
void SetMap(state *ptr, int w, int h, char c) {
	if (w<0 || w>=ptr->iMapWidth) 
		return;
	if (h<0 || h>=ptr->iMapHeight) 
		return;
	ptr->Map[w + h*ptr->iMapWidth] = c;
}
int GetMap(state *ptr, int w, int h, char *c) {
	if (w<0 || w>=ptr->iMapWidth)
		return 0;
	if (h<0 || h>=ptr->iMapHeight) 
		return 0;
	*c = ptr->Map[w + h*ptr->iMapWidth];
	return 1;
}
int IsOpenPos(state *ptr, int x, int y) {
	char c;
	if (GetMap(ptr, x, y, &c))
		return IsOpenArea(c);
	return 0;
}
int IsOpenArea(char c) {
	if (c=='P' || c==' ' || c=='.' || c=='$')
		return 1;
	return 0;
}
void RepopulateBoard(state *ptr) {
	int i;
	ptr->iMapWidth = 28;
	ptr->iMapHeight = 24;
	memcpy(ptr->Map, g_pLevel1Map,ptr->iMapWidth*ptr->iMapHeight);
	ptr->iDotsLeft = 0;
	for(i=ptr->iMapWidth*ptr->iMapHeight;i>=0;)
		if (ptr->Map[--i] == '.')
			ptr->iDotsLeft++;
}
static void DrawDynamic(void *pCtx, state *ptr) {
	DrawBoard(pCtx, ptr);
	DrawPills(pCtx, ptr);
	DrawPlayer(pCtx, ptr);
	DrawGhosts(pCtx, ptr);
	RenderRHS(pCtx, ptr);
}
tGameEnd GameLoop(void *pCtx, state *ptr) {
	int c, keypress;
	float telaps;
	struct timeval iTk, iTk2, tvTelaps;
	do {
		keypress = ptr->Player.iLastValidDir;
		gettimeofday(&iTk, 0);
		/* Wait 1/10 sec , getting whatever key's been pressed */
		do {
			c = getch();
			if (c != EOF)
			if (c != ptr->Player.iLastValidDir)
				keypress = c;
			if (c=='q' || c=='Q')
				return ePAC_UserQuit;
			gettimeofday(&iTk2, 0);
			timersub(&iTk2, &iTk, &tvTelaps);
			telaps = tvTelaps.tv_sec + tvTelaps.tv_usec/1000;
			telaps /= 1000.0f;
			#if _DEBUG
			move(21,30);
			printw(" clock=%ld telaps=%f key=%c last=%c", clock(), telaps, keypress, ptr->Player.iLastValidDir);
			move(23,30); 
			printw(" pills=%d", ptr->iDotsLeft);
			printw("tv %d + %d/1000", tvTelaps.tv_sec, tvTelaps.tv_usec);
			#endif
			if (telaps < 0) { 
				fprintf(stderr, "how?????"); 
				break; 
			}
		} while(telaps < 0.1f);
		/* scroll marquee */
		if (ptr->pMarquee) {
			c = *ptr->pMarquee;
			memmove(ptr->pMarquee, ptr->pMarquee+1, ptr->iMarqueeSize-1);
			ptr->pMarquee[ptr->iMarqueeSize-1] = c;
		}
		/* Update all game components */
		UpdatePlayer(ptr, telaps, keypress);
		CheckPlayerVsGhosts(ptr);
		UpdateGhosts(ptr, telaps);
		CheckPlayerVsGhosts(ptr);
		UpdatePills(ptr, telaps);
		/* Draw everything */
		DrawDynamic(pCtx, ptr);
		/* Make it visible */
		Blit(pCtx);
		/* Has an end game condition been reached? */
		if (ptr->iDotsLeft == 0) 
			return ePAC_SheetComplete;
		else if (ptr->Player.bDead) 
			return ePAC_LifeLost;
	} while(1);
}
void MainGame(void *pCtx, state *ptr) {
	tGameEnd iGS;
	InitialiseGame(ptr);
	RenderGameInfo(pCtx);
	RenderRHS(pCtx, ptr);
	do {
		/* Start of sheet/new life */
		DrawDynamic(pCtx, ptr);
		Blit(pCtx);
		sleep(2);
		iGS = GameLoop(pCtx, ptr);
		switch(iGS) {
			case	ePAC_SheetComplete:
				FlashBoard(pCtx, ptr);
				ptr->iLevel++;
				ReinitialiseGame(ptr);
			break;
			case	ePAC_LifeLost:
				--ptr->Player.iLives;
				RenderRHS(pCtx, ptr);
				AnimateDeadPlayer(pCtx, ptr);
				if (ptr->Player.iLives < 0)
					return;
				sleep(1);
				InitialiseGhosts(ptr);
				ReinitialisePlayer(ptr);
			break;
			default:
			break;
		}
	} while(iGS != ePAC_UserQuit);
}
