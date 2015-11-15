#define _RENDER_H
void *InitRender(void);
int CloseRender(void *);
void Blit(void *);
int DrawPills(void *, state *ptr);
void FlashBoard(void *, state *ptr);
int DrawBoard(void *, state *ptr);
int DrawPlayer(void *, state *ptr);
int DrawGhosts(void *, state *ptr);
int DrawEndGame(void *, state *ptr);
int AnotherGame(void *);
void AnimateDeadPlayer(void *, state *ptr);
void RenderGameInfo(void *);
void RenderRHS(void *, state *ptr);
<<<<<<< HEAD

=======
>>>>>>> e2d53ac58897bbc770e0364ccf47d413d0f92a93
