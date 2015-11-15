#define _PLAYER_H
int InitialisePlayer(state *ptr);
int ReinitialisePlayer(state *ptr);
void UpdatePlayer(state *ptr, float telaps, int key);
void CheckPlayerVsGhosts(state *ptr);
