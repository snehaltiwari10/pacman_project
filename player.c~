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

#include <ctype.h>
#include "pacman.h"
#include "player.h"
#include "game.h"
#include "lives.h"
static G_PEPPILL *GotPills(state *ptr, int x, int y) {
	int i;
	for(i=0; i<MAX_PILLS; i++)
		if (ptr->Pills[i].Pos.x == x && ptr->Pills[i].Pos.y == y && !ptr->Pills[i].bEaten)
			return &ptr->Pills[i];
	return 0;
}
int InitialisePlayer(state *ptr) {
	ptr->Player.iLives = 3;
	ReinitialisePlayer(ptr);
	return 0;
}
int ReinitialisePlayer(state *ptr) {
	G_PLAYER *pPlayer=&ptr->Player;
	pPlayer->bDead = 0;
	pPlayer->Pos.x = 14;
	pPlayer->Pos.y = 18;
	pPlayer->iLastValidDir = PACKEY_LEFT;
	pPlayer->fTimecum = 0;
	pPlayer->iAnimFrame = 0;
	return 0;
}
void CheckPlayerVsGhosts(state *ptr) {
	ghost *pG = &ptr->Ghosts[0];
	int i;
	for(i = 0; i < MAX_GHOSTS; i++, pG++)
		if (pG->bActive &&
		pG->Pos.x == ptr->Player.Pos.x &&
		pG->Pos.y == ptr->Player.Pos.y) {
			if (pG->fEdibleDuration == 0) {
				ptr->Player.bDead = 1;
			}
			else {
				ptr->Player.iScore += ptr->Player.iNextGhostScore;
				ptr->Player.iNextGhostScore *= 2;

				ptr->Ghosts[i].bActive = 0;
			}
		}
}
void UpdatePlayer(state *ptr, float telaps, int key) {
	int x = ptr->Player.Pos.x;
	int y = ptr->Player.Pos.y;
	int iPrevScore = ptr->Player.iScore;
	G_PEPPILL *pGot;
	char c;
	ptr->Player.fTimecum += telaps;
	while (ptr->Player.fTimecum > 0.1f) {
		ptr->Player.fTimecum -= 0.1f;
		if (++ptr->Player.iAnimFrame == 6)
			ptr->Player.iAnimFrame = 0;
	}
	key = tolower(key);
	x += (key==PACKEY_RIGHT?1:(key==PACKEY_LEFT?-1:0));
	y += (key==PACKEY_DOWN?1:(key==PACKEY_UP?-1:0));
	if (!IsOpenPos(ptr, x,y) || (x==ptr->Player.Pos.x && y==ptr->Player.Pos.y)) {
		key = ptr->Player.iLastValidDir;
		x = ptr->Player.Pos.x;
		y = ptr->Player.Pos.y;
		x += (key==PACKEY_RIGHT?1:(key==PACKEY_LEFT?-1:0));
		y += (key==PACKEY_DOWN?1:(key==PACKEY_UP?-1:0));
	}
	if (GetMap(ptr, x, y, &c)) {
		if (IsOpenArea(c)) {
			if (key==PACKEY_LEFT || key==PACKEY_RIGHT || key==PACKEY_UP || key==PACKEY_DOWN)
				ptr->Player.iLastValidDir = key;
			ptr->Player.Pos.x = x;
			ptr->Player.Pos.y = y;
			if (c == '.') {
				ptr->Player.iScore += 10;
				ptr->iDotsLeft--;
				SetMap(ptr, x, y, ' ');
			}
			if (c == 'P' && (pGot = GotPills(ptr, x,y))) {
				GhostsAreVunerable(ptr, pGot->fDuration);
				pGot->bEaten = 1;
				ptr->Player.iScore += 50;
				ptr->Player.iNextGhostScore = 200;
			}
		}
	}
	else if (x < 0) {
		/* must be the tunnel! */
		ptr->Player.Pos.x = ptr->iMapWidth-1;
	}
	else if (x >= ptr->iMapWidth) {
		ptr->Player.Pos.x = 0;
	}
	/* Bonus at 10K */
	if (iPrevScore < 10000 && ptr->Player.iScore >= 10000)
	if (++ptr->Player.iLives > MAX_PACMAN_LIVES)
	ptr->Player.iLives = MAX_PACMAN_LIVES;
	/* Special (i.e. cherries, et al) eaten? */
	if (ptr->Player.Pos.x == ptr->SpecialPos.x &&
	ptr->Player.Pos.y == ptr->SpecialPos.y &&
	ptr->fSpecialDuration) {
		ptr->Player.iScore += 100 + ptr->iSpecialChar*100;
		if (++ptr->iSpecialChar >= ptr->iLevel)
			ptr->iSpecialChar = 0;
		RespawnSpecial(ptr);
	}
	/* High? */
	if (ptr->Player.iScore > ptr->iHighScore)
		ptr->iHighScore = ptr->Player.iScore;
}
