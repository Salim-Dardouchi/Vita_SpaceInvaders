/*
 * bomb.c
 *
 *  Created on: 12 avr. 2021
 *      Author: Salim
 */


#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include <container.h>

#include "bomb.h"


/*------------------------STATIC IMPLEMENTATION SECTION--------------------------------------*/
static struct{
	SDL_Renderer*	pRenderer;
	SDL_Texture*	pImage;
	SDL_Rect*		pSceneArea;
}graph;

int BombInitGraph(SDL_Rect*pSceneArea,SDL_Renderer*pRenderer,char*bombImageFileStr){
	graph.pRenderer=pRenderer;
	graph.pImage=IMG_LoadTexture(graph.pRenderer, bombImageFileStr);
	graph.pSceneArea=pSceneArea;
	return 0;
}

int BombReleaseGraph(void){
	SDL_DestroyTexture(graph.pImage);
	return 0;
}




/*-----------------------DYNAMIC IMPLEMENTATION SECTION--------------------------------------*/
struct s_bomb {
	SDL_Rect	frame;
	SDL_Point	speed;
};

t_bomb*BombNew(SDL_Rect*pFrame,SDL_Point*pSpeed){
	t_bomb* pNewBomb=(t_bomb*)malloc(sizeof(t_bomb));
	pNewBomb->frame=*pFrame;
	pNewBomb->speed=*pSpeed;

	return pNewBomb;
}

t_bomb*BombDel(t_bomb*pBomb){
	free(pBomb);
	return NULL;
}


t_bomb*BombDraw(t_bomb*pBomb){
	SDL_RenderCopy(graph.pRenderer, graph.pImage, NULL, &pBomb->frame);
	return NULL;
}


t_bomb*BombMove(t_bomb*pBomb){
	pBomb->frame.y+=pBomb->speed.y;
	if((pBomb->frame.y+pBomb->frame.h+1)>=(graph.pSceneArea->y+graph.pSceneArea->h))return pBomb;
	return NULL;
}
t_bomb*BombCollide(t_bomb*pBomb, t_bomb*pOther, t_ptf pCallbackFunc){
	if (
			(pBomb->frame.x>pOther->frame.x) &&
			(pBomb->frame.x<pOther->frame.x+pOther->frame.w) &&
			(pBomb->frame.y>pOther->frame.y) &&
			(pBomb->frame.y<pOther->frame.y+pOther->frame.h)){
		pCallbackFunc();
		return pBomb;
	}
	return NULL;
}

