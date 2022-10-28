/*
 * rocket.h
 *
 *  Created on: 12 avr. 2021
 *      Author: Salim
 */
#include <container.h>
/*------------------------STATIC PROTOTYPE SECTION--------------------------------------*/
int RocketInitGraph(SDL_Rect*pSceneArea,SDL_Renderer*pRenderer,char*rocketImageFileStr);
int RocketReleaseGraph(void);

/*-----------------------DYNAMIC PROTOTYPE SECTION--------------------------------------*/
typedef struct s_rocket t_rocket;

t_rocket*RocketNew(SDL_Rect*pFrame,SDL_Point*pSpeed);
t_rocket*RocketDel(t_rocket*pRocket);
t_rocket*RocketDraw(t_rocket*pRocket);
t_rocket*RocketMove(t_rocket*pRocket);
t_rocket*RocketCollide(t_rocket*pRocket, t_rocket*pOther, t_ptf pCallbackFunc);
