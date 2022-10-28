/*
 * alien.c
 *
 *  Created on: 5 avr. 2021
 *      Author: Thierry
 */

#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include <container.h>
#include "alien.h"

#define ALIEN_LIMIT_Y_POS		(0.8)

/*------------------------STATIC IMPLEMENTATION SECTION--------------------------------------*/
static struct{
	SDL_Renderer*	pRenderer;
	SDL_Texture*	pImage;
	SDL_Rect		rSceneArea;
}graph;

struct{
	SDL_Rect	frame;
	SDL_Point	speed;
}s_alien;

int AlienInitGraph(SDL_Rect*pSceneArea,SDL_Renderer*pRenderer,char*alienImageFileStr){
	graph.pRenderer=pRenderer;
	graph.pImage=IMG_LoadTexture(graph.pRenderer,alienImageFileStr);
	graph.rSceneArea=*pSceneArea;
	return 0;
}

int AlienReleaseGraph(void){
	SDL_DestroyTexture(graph.pImage);
	return 0;
}

/*-----------------------DYNAMIC IMPLEMENTATION SECTION--------------------------------------*/
struct s_alien {
	SDL_Rect	frame;
	SDL_Point	speed;
};

t_alien*AlienNew(SDL_Rect*pFrame,SDL_Point*pSpeed){
	t_alien* pNewAlien=(t_alien*)malloc(sizeof(t_alien));
	pNewAlien->frame=*pFrame;
	pNewAlien->speed=*pSpeed;

	return pNewAlien;
}

t_alien*AlienDel(t_alien*pAlien){
	free(pAlien);
	return NULL;
}

t_alien*AlienDraw(t_alien*pAlien){
	SDL_RenderCopy(graph.pRenderer, graph.pImage, NULL, &pAlien->frame);
	return NULL;
}


t_alien*AlienMove(t_alien*pAlien){
	pAlien->frame.x+=pAlien->speed.x;
	return NULL;
}

t_alien*AlienScroll(t_alien*pAlien){
	pAlien->speed.x*=-1;
	pAlien->frame.y+=pAlien->speed.y;
	return NULL;
}

t_alien* AlienIsAtSceneSides(t_alien*pAlien){
	if((pAlien->frame.x<=graph.rSceneArea.x) || ((pAlien->frame.x+pAlien->frame.w)>=(graph.rSceneArea.x+graph.rSceneArea.w)))return pAlien;
	return NULL;
}

t_alien* AlienIsBeyondShip(t_alien*pAlien, t_alien*pOther){
	if((pAlien->frame.y+pAlien->frame.h)>(pOther->frame.y+pOther->frame.h/2))return pAlien;
	return NULL;
}

SDL_Point AlienGetBottomPoint(t_alien*pAlien){
	return (SDL_Point){pAlien->frame.x+pAlien->frame.w/2, pAlien->frame.y+pAlien->frame.h};
}
