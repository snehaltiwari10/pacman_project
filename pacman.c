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

#include <stdio.h>
#include <ctype.h>
#include "pacman.h"
#include "render.h"
#include "game.h"
void GetMarqueeText(state *ptr) {
	FILE *fp = fopen("pactext", "r");
	int i;
	char *pc;
	ptr->pMarquee = 0;
	if (fp) {
		fseek(fp, 0, SEEK_END);
		ptr->iMarqueeSize = ftell(fp);
		if ((ptr->pMarquee = malloc(ptr->iMarqueeSize))) {
			fseek(fp,0,SEEK_SET);
			fread(ptr->pMarquee, 1, ptr->iMarqueeSize, fp);
			pc = ptr->pMarquee;
			for(i=0; i<ptr->iMarqueeSize; i++, pc++)
				if (isalnum(*pc) || ispunct(*pc))
					;/*empty*/
				else
					*pc = ' ';
		}
		fclose(fp);
	}
}
int main(int argc, char **argv) {
	state game;
	void *pRender;
	game.iHighScore = 9710;
	GetMarqueeText(&game);
	pRender = InitRender();
	do {
		MainGame(pRender, &game);
		DrawEndGame(pRender, &game);
	} while (AnotherGame(pRender));
		CloseRender(pRender);
	return 0;
}
