/*
 * app.c
 *
 *  Created on: 5 avr. 2021
 *      Author: Thierry
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <debugScreen.h>

#include <scene.h>

#include <defines.h>
#include "app.h"

#define APP_SCENE_PADDING_TOP	64
#define	APP_SCENE_PADDING_BOT	32
#define APP_SCENE_PADDING_HRZ	32

#define APP_TICKRATE			17

#define APP_COLOR_WHITE			(0xFFFFFFFF)

/*----- Error management flags -----*/
typedef enum e_error{
	ST_APP_SDL_INIT_FAILED	=	0x1,
	ST_APP_SDL_WIND_FAILED	=	0x2,
	ST_APP_SDL_REND_FAILED	=	0x4,
	ST_APP_NEW_SUCCESS		=	0x0,
}t_stError;

//-Error management macros-------------

/*---- Main private app structure -----*/
struct s_app {
	SDL_Window*								pWindow;
	SDL_Renderer*							pRenderer;
	SDL_GameController*						pGameController;
	TTF_Font*								pFont;
	t_stError								uStError;
	t_scene*								pScene;
	int 									iWidth,iHeight;
};

/*----- Public functions -----*/
t_app*AppNew(char*appTitleStr, int iWidth, int iHeight){
	t_app* pNewApp = (t_app*)malloc(sizeof(t_app));
	mBitsSet(pNewApp->uStError, ST_APP_NEW_SUCCESS);

	if(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_GAMECONTROLLER)!=0){
		//fprintf(stderr, "SDL Initialization failed: %s\n", SDL_GetError());
		mBitsSet(pNewApp->uStError, ST_APP_SDL_INIT_FAILED);
	}
	pNewApp->iWidth=iWidth;
	pNewApp->iHeight=iHeight;

	pNewApp->pWindow=SDL_CreateWindow
			(
			appTitleStr,
			SDL_WINDOWPOS_UNDEFINED,
			SDL_WINDOWPOS_UNDEFINED,
			pNewApp->iWidth,
			pNewApp->iHeight,
			SDL_WINDOW_SHOWN);
	if(!pNewApp->pWindow){
		//fprintf(stderr, "SDL Window Creation failed: %s\n", SDL_GetError());
		mBitsSet(pNewApp->uStError,ST_APP_SDL_WIND_FAILED);
	}


	pNewApp->pRenderer=SDL_CreateRenderer(pNewApp->pWindow,-1, 0);
	if(!pNewApp->pRenderer){
		pNewApp->pRenderer=SDL_CreateRenderer(pNewApp->pWindow, -1, SDL_RENDERER_SOFTWARE);
		if(!pNewApp->pRenderer){
			//fprintf(stderr, "SDL Renderer Creation failed: %s\n", SDL_GetError());
			mBitsSet(pNewApp->uStError, ST_APP_SDL_REND_FAILED);
		}
	}
	
	pNewApp->pGameController = SDL_GameControllerOpen(0);

	SDL_Color cWhite;
	mToColor(cWhite,APP_COLOR_WHITE);

	pNewApp->pScene=SceneNew(
			&(SDL_Rect){APP_SCENE_PADDING_HRZ,
			APP_SCENE_PADDING_TOP,
			iWidth-(2*APP_SCENE_PADDING_HRZ),
			iHeight-(APP_SCENE_PADDING_TOP+APP_SCENE_PADDING_BOT)},
			pNewApp->pRenderer,
			&cWhite);

	return pNewApp;
}

t_app*AppDel(t_app*pApp){
	SceneDel(pApp->pScene);
	SDL_GameControllerClose(pApp->pGameController);
	SDL_DestroyRenderer(pApp->pRenderer);
	SDL_DestroyWindow(pApp->pWindow);
	SDL_Quit();
	free(pApp);
	return NULL;
}

int AppRun(t_app*pApp){
	Sint16 iLeftStickPos;
    unsigned int lastTime = 0, currentTime;
	int quit = 0;
	SDL_Event event;

	if (!mIsBitsSet(pApp->uStError, ST_APP_NEW_SUCCESS))
		return EXIT_FAILURE;

	SceneDraw(pApp->pScene);

	while(!quit){
		//Tick-based frame management.
        currentTime = SDL_GetTicks();
		if(!SceneGetGamestate(pApp->pScene)){
			//Rendering section
			if(currentTime>(lastTime + APP_TICKRATE)){
				SceneAnimate(pApp->pScene);
				lastTime=currentTime;

				//Input handling section
				if(SDL_GameControllerGetButton(pApp->pGameController, SDL_CONTROLLER_BUTTON_A))
					SceneSpacebarAction(pApp->pScene);
				if(SDL_GameControllerGetButton(pApp->pGameController, SDL_CONTROLLER_BUTTON_START))
					quit=1;
				if(SDL_GameControllerGetButton(pApp->pGameController, SDL_CONTROLLER_BUTTON_DPAD_LEFT))
					SceneShipMoveJoystickDirection(pApp->pScene, 0);
				if(SDL_GameControllerGetButton(pApp->pGameController, SDL_CONTROLLER_BUTTON_DPAD_RIGHT))
					SceneShipMoveJoystickDirection(pApp->pScene, 1);
				if(iLeftStickPos = SDL_GameControllerGetAxis(pApp->pGameController, SDL_CONTROLLER_AXIS_LEFTX)){
					if(iLeftStickPos<(-2048))
						SceneShipMoveJoystickDirection(pApp->pScene, 0);
					else if(iLeftStickPos>2048)
						SceneShipMoveJoystickDirection(pApp->pScene, 1);
				}
				SDL_GameControllerUpdate();
			}
        }
		else{
			if(SDL_GameControllerGetButton(pApp->pGameController, SDL_CONTROLLER_BUTTON_START))
				quit=1;
			SDL_GameControllerUpdate();
		}
	}
	return 0;
}

				