/*
 * ship.c
 *
 *  Created on: 12 avr. 2021
 *      Author: Salim
 */

#include <stdlib.h>
#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#define SHIP_Y_POSITION		(0.8) //This means that our ship is always located at 4/5th of the SceneArea

#include "ship.h"
#include <defines.h>

#define SHIP_SIZE		(32)

typedef enum eShipStatus{
	ST_IS_PLAYABLE =  0x1,
	ST_NOT_PLAYABLE = 0x2,
}tShipStatus;

static struct{
	SDL_Renderer*	pRenderer;
	SDL_Texture*	pImage;
	SDL_Rect*		pSceneArea;
	tShipStatus		uPlayable;
}graph;

struct{
	SDL_Rect	frame;
	SDL_Point	speed;
}s_ship;

int ShipInitGraph(SDL_Rect*pSceneArea,SDL_Renderer*pRenderer,char*shipImageFileStr){
	graph.pRenderer=pRenderer;
	graph.pImage=IMG_LoadTexture(graph.pRenderer,shipImageFileStr);
	graph.pSceneArea=pSceneArea;
	mBitsSet(graph.uPlayable, ST_NOT_PLAYABLE);
	return 0;
}

int ShipReleaseGraph(void){
	SDL_DestroyTexture(graph.pImage);
	graph.pRenderer=NULL;
	return 0;
}


/*-----------------------DYNAMIC IMPLEMENTATION SECTION--------------------------------------*/
struct s_ship {
	SDL_Rect	frame;
	SDL_Point	speed;
};

t_ship*ShipNew(SDL_Rect*pFrame,SDL_Point*pSpeed){
	t_ship* pNewShip=(t_ship*)malloc(sizeof(t_ship));
	pNewShip->frame=*pFrame;
	pNewShip->speed=*pSpeed;
	return pNewShip;
}

t_ship*ShipDel(t_ship*pShip){
	free(pShip);
	return NULL;
}

t_ship*ShipDraw(t_ship*pShip){
	SDL_RenderCopy(graph.pRenderer, graph.pImage, NULL , &pShip->frame);
	return NULL;
}

t_ship*ShipMove(t_ship*pShip, int iX){
	if( (iX-(pShip->frame.w/2)>graph.pSceneArea->x) && ((iX+(pShip->frame.w/2)<graph.pSceneArea->x+graph.pSceneArea->w)) )pShip->frame.x=iX-(pShip->frame.w/2);

	return NULL;
}

SDL_Point ShipGetHotpoint(t_ship*pShip){
	SDL_Point hotpoint = {pShip->frame.x+pShip->frame.w/2, pShip->frame.y};
	return hotpoint;
}

int ShipIsPlayable(){
	return mIsBitsSet(graph.uPlayable, ST_IS_PLAYABLE);
}

void ShipSetPlayable(t_ship* pShip, int iX){
	mBitsSet(graph.uPlayable, ST_IS_PLAYABLE);
	pShip->frame=(SDL_Rect){iX-(SHIP_SIZE/2),(graph.pSceneArea->y+graph.pSceneArea->h*SHIP_Y_POSITION),SHIP_SIZE,SHIP_SIZE};
}

void ShipIncrementPosX(t_ship* pShip, int iSpeed){
	if( (pShip->frame.x+iSpeed>graph.pSceneArea->x) &&  (pShip->frame.x+pShip->frame.w+iSpeed<(graph.pSceneArea->w+graph.pSceneArea->x)) )pShip->frame.x+=iSpeed;
}
