#define _PACMAN_H
#include <stdlib.h>
#define TRUE	1
#define FALSE	0
#define MAX_PILLS	4
#define MAX_GHOSTS	4
#define MAX_PACMAN_LIVES	6
#define PACKEY_UP	'k'
#define PACKEY_DOWN	'm'
#define PACKEY_LEFT	'x'
#define PACKEY_RIGHT	'c'
#define RND(__x) (int)((double)rand()/((double)RAND_MAX+1)*__x)
typedef int BOOL;
typedef enum {
	ePAC_LifeLost,
	ePAC_SheetComplete,
	ePAC_UserQuit,
} tGameEnd;
typedef enum {
	eDIR_Left,
	eDIR_Right,
	eDIR_Up,
	eDIR_Down,
} tDir;
typedef struct sGPOS {
	int x, y;
} GPOS;
typedef struct sGPILL {
	GPOS Pos;
	int bEaten;
	float fDuration;
} G_PEPPILL;
typedef struct sGHOST {
	GPOS Pos;
	int bActive;
	int iInHome;
	float fEdibleDuration;
	tDir Direction;
} ghost;
typedef struct sGPLAYER {
	GPOS Pos;
	int bDead;
	int iScore;
	int iLives;
	int iLastValidDir;
	float fTimecum;
	int iAnimFrame;
	int iNextGhostScore;
} G_PLAYER;
typedef struct sGAMESTATE {
	float iPP_Timecum;
	int iPP_Flash;
	G_PEPPILL Pills[MAX_PILLS];
	float fTimeBeforeSpecial;
	float fSpecialDuration;
	int iSpecialChar;
	GPOS SpecialPos;
	ghost Ghosts[MAX_GHOSTS];
	G_PLAYER Player;
	int iLevel;
	int iDotsLeft;
	int iMapWidth, iMapHeight;
	char Map[672];
	int iHighScore;
	char *pMarquee;
	long iMarqueeSize;
} state;

