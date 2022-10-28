/*
 * rocket.c
 *
 *  Created on: 12 avr. 2021
 *      Author: Salim
 */



#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "rocket.h"
#include <container.h>

/*------------------------STATIC IMPLEMENTATION SECTION--------------------------------------*/
static struct{
	SDL_Renderer*	pRenderer;
	SDL_Texture*	pImage;
	SDL_Rect*		pSceneArea;
}graph;

int RocketInitGraph(SDL_Rect*pSceneArea,SDL_Renderer*pRenderer,char*rocketImageFileStr){
	graph.pRenderer=pRenderer;
	graph.pImage=IMG_LoadTexture(graph.pRenderer, rocketImageFileStr);
	graph.pSceneArea=pSceneArea;
	return 0;
}

int RocketReleaseGraph(void){
	SDL_DestroyTexture(graph.pImage);
	return 0;
}




/*-----------------------DYNAMIC IMPLEMENTATION SECTION--------------------------------------*/
struct s_rocket {
	SDL_Rect	frame;
	SDL_Point	speed;
};

t_rocket*RocketNew(SDL_Rect*pFrame,SDL_Point*pSpeed){
	t_rocket* pNewRocket=(t_rocket*)malloc(sizeof(t_rocket));
	pNewRocket->frame=*pFrame;
	pNewRocket->speed=*pSpeed;

	return pNewRocket;
}

t_rocket*RocketDel(t_rocket*pRocket){
	free(pRocket);
	return NULL;
}


t_rocket*RocketDraw(t_rocket*pRocket){
	SDL_RenderCopy(graph.pRenderer, graph.pImage, NULL, &pRocket->frame);
	return NULL;
}

t_rocket*RocketMove(t_rocket*pRocket){
	pRocket->frame.y+=pRocket->speed.y;
	if(pRocket->frame.y<=graph.pSceneArea->y){
		return pRocket;
	}
	return NULL;
}

t_rocket*RocketCollide(t_rocket*pRocket, t_rocket*pOther, t_ptf pCallbackFunc){
	if (
			(pRocket->frame.x>=pOther->frame.x) &&
			(pRocket->frame.x<=pOther->frame.x+pOther->frame.w) &&
			(pRocket->frame.y>=pOther->frame.y) &&
			(pRocket->frame.y<=pOther->frame.y+pOther->frame.h)){
		pCallbackFunc();
		return pRocket;
	}
	return NULL;
}
