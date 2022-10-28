/*
 * alien.h
 *
 *  Created on: 5 avr. 2021
 *      Author: Thierry
 */

#include <container.h>

/*------------------------STATIC PROTOTYPE SECTION--------------------------------------*/
int AlienInitGraph(SDL_Rect*pSceneArea,SDL_Renderer*pRenderer,char*alienImageFileStr);
int AlienReleaseGraph(void);

/*-----------------------DYNAMIC PROTOTYPE SECTION--------------------------------------*/
typedef struct s_alien t_alien;

t_alien*AlienNew(SDL_Rect*pFrame,SDL_Point*pSpeed);
t_alien*AlienDel(t_alien*pAlien);
t_alien*AlienDraw(t_alien*pAlien);
t_alien*AlienMove(t_alien*pAlien);
t_alien*AlienScroll(t_alien*pAlien);
t_alien*AlienIsAtSceneSides(t_alien*pAlien);
t_alien* AlienIsBeyondShip(t_alien*pAlien, t_alien*pOther);
SDL_Point AlienGetBottomPoint(t_alien*pAlien);
