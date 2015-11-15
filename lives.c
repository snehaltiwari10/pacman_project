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
#include <stdlib.h>
#include "pacman.h"
#include "lives.h"
#include "game.h"
#define GIH_SHUFFLE	1
#define GIH_GATEWAY	2
#define GIH_OUTSIDE	0
int InitialisePills(state *ptr) {
	int x,y;
	int i;
	char c;
	ptr->iPP_Flash = ptr->iPP_Timecum = 0;
	ptr->iSpecialChar = ptr->iLevel;
	ptr->SpecialPos.x = ptr->SpecialPos.y = 14;
	RespawnSpecial(ptr);
	i = 0;
	for(y = 0; y < ptr->iMapHeight; y++)
		for(x = 0; x < ptr->iMapWidth; x++)
			if (GetMap(ptr, x, y, &c) && c=='P') {
				if (i < MAX_PILLS) {
					ptr->Pills[i].bEaten = 0;
					ptr->Pills[i].Pos.x = x;
					ptr->Pills[i].Pos.y = y;
					ptr->Pills[i].fDuration = 6;
					i++;
				}
				else {
					fprintf(stderr, "Too many power pills in this level\n");
				}
			}
	return 0;
}
int UpdatePills(state *ptr, float telaps) {
	ptr->iPP_Timecum += telaps;
	if (ptr->iPP_Timecum > 0.5f) {
		ptr->iPP_Timecum = 0;
		ptr->iPP_Flash ^= 1;
	}
	if (ptr->fSpecialDuration) {
		ptr->fSpecialDuration -= telaps;
		if (ptr->fSpecialDuration < 0)
			RespawnSpecial(ptr);
	}
	else {
		ptr->fTimeBeforeSpecial -= telaps;
		if (ptr->fTimeBeforeSpecial < 0) {
			ptr->fTimeBeforeSpecial = 0;
			ptr->fSpecialDuration = 10 + RND(3);
		}
	}
	return 0;
}
int RespawnSpecial(state *ptr) {
	ptr->fTimeBeforeSpecial = 10 + RND(4)*(5+ptr->iLevel);
	ptr->fSpecialDuration = 0;
	return 0;
}
static char GetNextChar(state *ptr, ghost *pGhost) {
	int x, y;
	char c;
	x = pGhost->Pos.x; y = pGhost->Pos.y;
	switch(pGhost->Direction) {
		case	eDIR_Left: x--;
		break;
		case	eDIR_Right: x++; 
		break;
		case	eDIR_Up: y--; 
		break;
		case	eDIR_Down: y++; 
		break;
	}
	if (GetMap(ptr, x, y, &c))
	return c;
	else
	return '\0';
}
static int IsNextMoveValid(state *ptr, ghost *pGhost) {
	char c;
	c = GetNextChar(ptr, pGhost);
	return IsOpenArea(c);
}
static void HomedGhost(state *ptr, ghost *pGhost) {
	char c;
	switch(pGhost->iInHome) {
		case GIH_SHUFFLE:
		/* Move up into gateway?? */
		if (RND(2) == 0 && GetMap(ptr,pGhost->Pos.x,pGhost->Pos.y-1,&c) && c=='-') {
			pGhost->iInHome = GIH_GATEWAY;
			pGhost->Pos.y--;
		}
		else {
			c = RND(3);
			if (c == 0 && GetMap(ptr,pGhost->Pos.x-1,pGhost->Pos.y,&c) && c=='H')
				pGhost->Pos.x--;
			else if (c == 1 && GetMap(ptr,pGhost->Pos.x+1,pGhost->Pos.y,&c) && c=='H')
				pGhost->Pos.x++;
		}
		break;
		case GIH_GATEWAY:
		pGhost->Pos.y--; /* ASSUME: gate opens upwards */
		pGhost->iInHome = GIH_OUTSIDE;
		if (RND(2) == 0)
			pGhost->Direction = eDIR_Left;
		else
			pGhost->Direction = eDIR_Right;
		break;
	}
}
static void MoveGhost(state *ptr, ghost *pGhost)
{
	int tries=0;
	if (!IsNextMoveValid(ptr, pGhost)) {
	 /* must be either 1 or 2 valid directions */
		tDir w1,w2;
		switch(pGhost->Direction)
		{
			case eDIR_Right:
			case eDIR_Left: w1=eDIR_Up, w2=eDIR_Down;
			break;
			case eDIR_Up:
			case eDIR_Down: w1=eDIR_Left, w2=eDIR_Right; 
			break;
		}
		if (RND(2)==0) {
			/* try w1 first, then w2 */
			pGhost->Direction = w1;
			if (!IsNextMoveValid(ptr, pGhost))
				pGhost->Direction = w2;
		}
		else {
			pGhost->Direction = w2;
			if (!IsNextMoveValid(ptr, pGhost))
				pGhost->Direction = w1;
		}
	}
	if (RND(34) == 0 && pGhost->Pos.x != ptr->Player.Pos.x && pGhost->Pos.y != ptr->Player.Pos.y) {
		pGhost->Direction = RND(4); /* a little bit bad! */
		while(++tries < 4) {
			if (IsNextMoveValid(ptr, pGhost))
				break;/* found a new dir */
			else
				if (++pGhost->Direction == 4)
					pGhost->Direction = 0;
		}
	}
	/* Move the ghost */
	switch(pGhost->Direction) {
		case eDIR_Left: pGhost->Pos.x--; 
		break;
		case eDIR_Right: pGhost->Pos.x++; 
		break;
		case eDIR_Up: pGhost->Pos.y--; 
		break;
		case eDIR_Down: pGhost->Pos.y++; 
		break;
	}
	/* Check for tunnel */
	if (pGhost->Pos.x < 0)
		pGhost->Pos.x = ptr->iMapWidth-1;
	if (pGhost->Pos.x >= ptr->iMapWidth) 
		pGhost->Pos.x = 0;
}
static void DormantGhost(state *ptr, ghost *pGhost) {
	/* Wake him up?? */
	if (RND(30) < 2)
		ActivateGhost(pGhost, 12,10,eDIR_Left);
}
int InitialiseGhosts(state *ptr) {
	int i;
	for(i=0; i<MAX_GHOSTS; i++)
		ptr->Ghosts[i].bActive = 0;
	ActivateGhost(&ptr->Ghosts[0], 12, 10, eDIR_Left);
	ActivateGhost(&ptr->Ghosts[1], 14, 10, eDIR_Left);
	ActivateGhost(&ptr->Ghosts[2], 16, 10, eDIR_Left);
	return 0;
}
char ActivateGhost(ghost *pGhost, int x, int y, tDir eDir) {
	if (pGhost->bActive) 
		return;
	pGhost->Pos.x = x;
	pGhost->Pos.y = y;
	pGhost->Direction = eDir;
	pGhost->bActive = 1;
	pGhost->fEdibleDuration = 0;
	pGhost->iInHome = GIH_SHUFFLE;
	return '\0';
}
int UpdateGhosts(state *ptr, float telaps) {
	int i;
	for(i=0; i<MAX_GHOSTS; i++) {
		ptr->Ghosts[i].fEdibleDuration -= telaps;
		if (ptr->Ghosts[i].fEdibleDuration < 0)
			ptr->Ghosts[i].fEdibleDuration = 0;
		if (ptr->Ghosts[i].bActive == 0)
			DormantGhost(ptr, &ptr->Ghosts[i]);
		else if (ptr->Ghosts[i].iInHome)
			HomedGhost(ptr, &ptr->Ghosts[i]);
		else
			MoveGhost(ptr, &ptr->Ghosts[i]);
	}
	return 0;
}
void GhostsAreVunerable(state *ptr, float fDuration) {
	int i;
	for(i=0; i<MAX_GHOSTS; i++)
		if (ptr->Ghosts[i].bActive && ptr->Ghosts[i].iInHome==GIH_OUTSIDE)
			ptr->Ghosts[i].fEdibleDuration += fDuration;
}
