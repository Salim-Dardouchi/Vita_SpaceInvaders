/*
 * ship.h
 *
 *  Created on: 12 avr. 2021
 *      Author: Salim
 */

/*------------------------STATIC PROTOTYPE SECTION--------------------------------------*/
int ShipInitGraph(SDL_Rect*pSceneArea,SDL_Renderer*pRenderer,char*shipImageFileStr);
int ShipReleaseGraph(void);

/*-----------------------DYNAMIC PROTOTYPE SECTION--------------------------------------*/
typedef struct s_ship t_ship;

t_ship*ShipNew(SDL_Rect*pFrame,SDL_Point*pSpeed);
t_ship*ShipDel(t_ship*pShip);
t_ship*ShipDraw(t_ship*pShip);
t_ship*ShipMove(t_ship*pShip, int iX);
SDL_Point ShipGetHotpoint(t_ship*pShip);
int ShipIsPlayable();
void ShipSetPlayable(t_ship* pShip, int iX);
void ShipIncrementPosX(t_ship* pShip, int iSpeed);