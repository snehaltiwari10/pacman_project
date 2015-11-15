#define _LIVES_H
int InitialisePills(state *ptr);
int UpdatePills(state *ptr, float telaps);
int RespawnSpecial(state *ptr);
int InitialiseGhosts(state *ptr);
int UpdateGhosts(state *ptr, float telaps);
char ActivateGhost(ghost *pGh, int x, int y, tDir eDir);
void GhostsAreVunerable(state *ptr, float fDuration);
