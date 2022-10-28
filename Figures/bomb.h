/*
 * bomb.h
 *
 *  Created on: 12 avr. 2021
 *      Author: Salim
 */

/*------------------------STATIC PROTOTYPE SECTION--------------------------------------*/
int BombInitGraph(SDL_Rect*pSceneArea,SDL_Renderer*pRenderer,char*bombImageFileStr);
int BombReleaseGraph(void);

/*-----------------------DYNAMIC PROTOTYPE SECTION--------------------------------------*/
typedef struct s_bomb t_bomb;

t_bomb*BombNew(SDL_Rect*pFrame,SDL_Point*pSpeed);
t_bomb*BombDel(t_bomb*pBomb);
t_bomb*BombDraw(t_bomb*pBomb);
t_bomb*BombMove(t_bomb*pBomb);
t_bomb*BombCollide(t_bomb*pBomb, t_bomb*pOther, t_ptf pCallbackFunc);
