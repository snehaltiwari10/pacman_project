#define _GAME_H
int InitialiseGame(state *ptr);
int ReinitialiseGame(state *ptr);
void RepopulateBoard(state *ptr);
void SetMap(state *ptr, int , int , char );
int GetMap(state *ptr, int , int , char *);
int IsOpenArea(char );
int IsOpenPos(state *ptr, int , int );
tGameEnd GameLoop(void *pCtx, state *ptr);
void MainGame(void *pCtx, state *ptr);
<<<<<<< HEAD

=======
>>>>>>> e2d53ac58897bbc770e0364ccf47d413d0f92a93
