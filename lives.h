#define _LIVES_H
int InitialisePills(state *ptr);
int UpdatePills(state *ptr, float telaps);
int RespawnSpecial(state *ptr);
int InitialiseGhosts(state *ptr);
int UpdateGhosts(state *ptr, float telaps);
char ActivateGhost(ghost *pGh, int x, int y, tDir eDir);
void GhostsAreVunerable(state *ptr, float fDuration);
<<<<<<< HEAD

=======
>>>>>>> e2d53ac58897bbc770e0364ccf47d413d0f92a93
