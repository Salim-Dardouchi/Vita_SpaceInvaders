/*
 * scene.c
 *
 *  Created on: 5 avr. 2021
 *      Author: Thierry
 */


#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>

#include <container.h>
#include <sound.h>
#include <defines.h>

#include <alien.h>
#include <ship.h>
#include <rocket.h>
#include <bomb.h>
#include <text.h>

#include "scene.h"


//-------------------- PRIVATE FUNCTION PROTOTYPES -------------------------------/
void _SceneChangeScore(t_scene*pScene);
void _SceneScore();
void _SceneDummyFunction();
void _SceneAlienCounterAttack(t_scene*pScene);
//--------------------------------------

typedef enum eGameState{
	ST_SCENE_GAME_PLAYING 	= 0x1,
	ST_SCENE_GAMEOVER_WIN 	= 0x2,
	ST_SCENE_GAMEOVER_LOSE 	= 0x4,
}t_GameState;

//------------------ GLOBAL VARIABLES FOR SCORING -------------------------------/
int		iScore=0, iShipPos=0;
//-------------------------------------------------------------------------------/

struct s_scene{
	SDL_Rect 		rSceneArea;
	SDL_Color		pColorBkgnd,
					pColorSceneArea;

	SDL_Renderer*	pRenderer;
	SDL_Texture*	pBackground;

	SDL_Texture*	pWinTexture;
	SDL_Texture*	pLoseTexture;
	SDL_Rect		rWinRect;
	SDL_Rect		rLoseRect;

	TTF_Font*		pScoreFont;
	TTF_Font*		pMsgFont;

	t_container*	pShips;
	t_container*	pRockets;
	t_container* 	pAliens;
	t_container* 	pBombs;
	t_container*	pTexts;

	t_GameState		uGameState;

	t_sound*		pSound;

	int 			iShipLives,
					iAliensLeft;
};

t_scene*SceneNew(SDL_Rect*rSceneArea,SDL_Renderer*pRenderer,SDL_Color*pColorBkgnd){
	t_scene* pNewSce = (t_scene*)malloc(sizeof(t_scene));
	pNewSce->rSceneArea=*rSceneArea;
	pNewSce->pColorBkgnd=*pColorBkgnd;
	pNewSce->pRenderer=pRenderer;
	pNewSce->uGameState=0;
	iScore = 0;

	mBitsSet(pNewSce->uGameState, ST_SCENE_GAME_PLAYING);

	srand((unsigned int)time(NULL));

	ShipInitGraph(&pNewSce->rSceneArea, pNewSce->pRenderer, SCENE_SHIP_IMG_PATH);
	RocketInitGraph(&pNewSce->rSceneArea, pNewSce->pRenderer, SCENE_ROCKET_IMG_PATH);
	AlienInitGraph(&pNewSce->rSceneArea, pNewSce->pRenderer, SCENE_ALIEN_IMG_PATH);
	BombInitGraph(&pNewSce->rSceneArea, pNewSce->pRenderer, SCENE_BOMB_IMG_PATH);

	pNewSce->pShips=ContainerNew((t_ptfV)ShipDel);
	pNewSce->pRockets=ContainerNew((t_ptfV)RocketDel);
	pNewSce->pAliens=ContainerNew((t_ptfV)AlienDel);
	pNewSce->pBombs=ContainerNew((t_ptfV)BombDel);
	pNewSce->pTexts=ContainerNew((t_ptfV)TextDel);

	for(int k=0; k<SCENE_SHIP_LIVES;k++){
		ContainerPushback(
				pNewSce->pShips,
				ShipNew
						(
						&(SDL_Rect){rSceneArea->w-((SCENE_SHIP_REDUCED_SIZE*k+1)+(SCENE_SHIP_REDUCED_PAD*k+1)),
						rSceneArea->y+rSceneArea->h-SCENE_SHIP_REDUCED_PAD-SCENE_SHIP_REDUCED_SIZE,
						SCENE_SHIP_REDUCED_SIZE,SCENE_SHIP_REDUCED_SIZE},
						&(SDL_Point){SCENE_SHIP_SPEED,SCENE_SHIP_SPEED}));
	}
	if(pNewSce->pShips!=NULL)
        printf("[scene.c]: Filled pShips container\n");
    else
        printf("[scene.c]: pShips contains NULL, app will crash!\n");

	pNewSce->iShipLives = ContainerCard(pNewSce->pShips);

	for(int k=0; k<SCENE_ALIEN_ROWS; k++){
		for(int m=0; m<SCENE_ALIEN_COLS; m++){
			ContainerPushback(
					pNewSce->pAliens,
					AlienNew(
							&(SDL_Rect){rSceneArea->x+SCENE_ALIEN_PADDING_HRZ+m*(SCENE_ALIEN_SPACING+SCENE_ALIEN_SIZE),
										rSceneArea->y+SCENE_ALIEN_PADDING_VRT+k*(SCENE_ALIEN_SPACING+SCENE_ALIEN_SIZE),
										SCENE_ALIEN_SIZE,SCENE_ALIEN_SIZE},
							&(SDL_Point){SCENE_ALIEN_SPEED_HRZ, SCENE_ALIEN_SPEED_VRT}));
		}
	}

	pNewSce->iAliensLeft = ContainerCard(pNewSce->pAliens);

	iShipPos = rSceneArea->x+(rSceneArea->w/2);
	ShipSetPlayable(ContainerGetback(pNewSce->pShips), iShipPos);

	pNewSce->pBackground=IMG_LoadTexture(pNewSce->pRenderer, SCENE_BACKGROUND_PATH);

	if(TTF_Init())
		fprintf(stderr,"Couldnt initialize TTF %s\n", TTF_GetError());

	pNewSce->pScoreFont=TTF_OpenFont(SCENE_FONT_PATH, SCENE_SCORE_FONT_SIZE);
	if(!pNewSce->pScoreFont)
		fprintf(stderr,"Couldn't load font pScoreFont %s\n", TTF_GetError());

	TextInitGraph(pNewSce->pScoreFont, (SDL_Color){255, 255, 255, 255}, pNewSce->pRenderer);
	_SceneChangeScore(pNewSce);

	pNewSce->pMsgFont=TTF_OpenFont(SCENE_FONT_PATH, SCENE_MSG_FONT_SIZE);
    if(!pNewSce->pMsgFont)
		fprintf(stderr,"Couldn't load font pMsgFont %s\n", TTF_GetError());

	SDL_Surface* pWinSur = TTF_RenderText_Blended(pNewSce->pMsgFont, "YOU WIN", (SDL_Color){255, 255, 255, 255});
	SDL_Surface* pLoseSur = TTF_RenderText_Blended(pNewSce->pMsgFont, "YOU LOSE", (SDL_Color){255, 255, 255, 255});
	
	pNewSce->pWinTexture = SDL_CreateTextureFromSurface(pNewSce->pRenderer, pWinSur);
	pNewSce->pLoseTexture = SDL_CreateTextureFromSurface(pNewSce->pRenderer, pLoseSur);

	int iWW, iWH, iLW, iLH;

	SDL_QueryTexture(pNewSce->pWinTexture, NULL, NULL, &iWW, &iWH);
	pNewSce->rWinRect = (SDL_Rect){
		rSceneArea->x+(rSceneArea->w/2)-(iWW/2),
		rSceneArea->y+(rSceneArea->h/2)-(iWH/2),
		iWW,
		iWH
	};

	SDL_QueryTexture(pNewSce->pWinTexture, NULL, NULL, &iLW, &iLH);
	pNewSce->rLoseRect = (SDL_Rect){
		rSceneArea->x+(rSceneArea->w/2)-(iLW/2),
		rSceneArea->y+(rSceneArea->h/2)-(iLH/2),
		iWW,
		iWH
	};

	SDL_FreeSurface(pLoseSur);
	SDL_FreeSurface(pWinSur);

	pNewSce->pSound=SoundNew();

	SoundPlayMusic(pNewSce->pSound);
	return pNewSce;
}

t_scene*SceneDel(t_scene*pScene){

	SoundHaltMusic(pScene->pSound);
	pScene->pSound=SoundDel(pScene->pSound);

	SDL_DestroyTexture(pScene->pLoseTexture);
	SDL_DestroyTexture(pScene->pWinTexture);

	if(pScene->pMsgFont)TTF_CloseFont(pScene->pMsgFont);

	if(pScene->pScoreFont)TTF_CloseFont(pScene->pScoreFont);

	TTF_Quit();

	pScene->pShips=ContainerDel(pScene->pTexts);
	pScene->pBombs=ContainerDel(pScene->pBombs);
	pScene->pAliens=ContainerDel(pScene->pAliens);
	pScene->pRockets=ContainerDel(pScene->pRockets);
	pScene->pShips=ContainerDel(pScene->pShips);


	if(pScene->pBackground)SDL_DestroyTexture(pScene->pBackground);

	BombReleaseGraph();
	AlienReleaseGraph();
	ShipReleaseGraph();
	RocketReleaseGraph();
	TextReleaseGraph();

	if(pScene)free(pScene);
	return NULL;
}

void SceneDraw(t_scene*pScene){
		//SDL_SetRenderDrawColor(pScene->pRenderer, 0, 0, 0, 255);
 		//SDL_RenderClear(pScene->pRenderer);
		/*I'm using these lines of code to disable the image background and using a pitchblack one, because for some reason the app crashes 4 times out of 5
		on my main computer if i'm not using this workaround. (I think it has something to do with my GPU's driver (launching the app already made my
		AMD drivers timeout).
		However it does work on my laptop and on a virtual machine just fine, it's just AMD Windows drivers being weird I guess....*/
		SDL_RenderCopy(pScene->pRenderer, pScene->pBackground, NULL, &(SDL_Rect){0,0,SCENE_BACKGROUND_RES_X,SCENE_BACKGROUND_RES_Y});

		ContainerParse(pScene->pAliens, (t_ptfVV)AlienDraw, NULL);
		ContainerParse(pScene->pRockets, (t_ptfVV)RocketDraw, NULL);
		ContainerParse(pScene->pBombs, (t_ptfVV)BombDraw, NULL);
		ContainerParse(pScene->pShips, (t_ptfVV)ShipDraw, NULL);
		if(ContainerCard(pScene->pTexts))TextDraw((t_text*)ContainerGetback(pScene->pTexts));
		SDL_RenderPresent(pScene->pRenderer);
}

void SceneAnimate(t_scene*pScene){
	 if(ContainerCard(pScene->pShips) && (ContainerParse(pScene->pAliens, (t_ptfVV)AlienIsBeyondShip, ContainerGetback(pScene->pShips)))){
		mBitsSet(pScene->uGameState, ST_SCENE_GAMEOVER_LOSE);

		SDL_RenderCopy(pScene->pRenderer, pScene->pLoseTexture, NULL,&pScene->rLoseRect);
		SDL_RenderPresent(pScene->pRenderer);
	 }
 
    if(!ContainerCard(pScene->pShips)){
        mBitsSet(pScene->uGameState, ST_SCENE_GAMEOVER_LOSE);

		SceneDraw(pScene);
		SDL_RenderCopy(pScene->pRenderer, pScene->pLoseTexture, NULL,&pScene->rLoseRect);
		SDL_RenderPresent(pScene->pRenderer);
	}

    if((!ContainerCard(pScene->pAliens))){
        mBitsSet(pScene->uGameState, ST_SCENE_GAMEOVER_WIN);

		SceneDraw(pScene);
		SDL_RenderCopy(pScene->pRenderer, pScene->pWinTexture, NULL,&pScene->rWinRect);
		SDL_RenderPresent(pScene->pRenderer);
	}

    if(pScene->uGameState==ST_SCENE_GAME_PLAYING){

        if(ContainerCard(pScene->pRockets))
            ContainerParseDelIf(pScene->pRockets, (t_ptfVV)RocketMove, NULL);

        if(ContainerCard(pScene->pShips) && (ContainerCard(pScene->pShips)!=pScene->iShipLives)){
            pScene->iShipLives=ContainerCard(pScene->pShips);
            SoundPlayShipKilledSound(pScene->pSound);
            if(pScene->iShipLives){
                ShipSetPlayable(ContainerGetback(pScene->pShips), iShipPos);
            }
        }
        if(ContainerCard(pScene->pBombs))
            ContainerParseDelIf(pScene->pBombs, (t_ptfVV)BombMove,  NULL);

        if(ContainerCard(pScene->pAliens)){
            ContainerParse(pScene->pAliens, (t_ptfVV)AlienMove, NULL);
            if(ContainerParse(pScene->pAliens, (t_ptfVV)AlienIsAtSceneSides, NULL)){
                ContainerParse(pScene->pAliens, (t_ptfVV)AlienScroll, NULL);
            }
        }

        if(ContainerCard(pScene->pRockets) && ContainerCard(pScene->pAliens)){
            ContainerIntersectDelIf(pScene->pRockets, pScene->pAliens, (t_ptfVVF)RocketCollide, (t_ptf)_SceneScore);
            if(pScene->iAliensLeft!=ContainerCard(pScene->pAliens)){
				_SceneChangeScore(pScene);
                SoundPlayAlienKilledSound(pScene->pSound);
                pScene->iAliensLeft=ContainerCard(pScene->pAliens);
            }
        }

        if(ContainerCard(pScene->pBombs) && ContainerCard(pScene->pShips)){
            ContainerIntersectDelIf(pScene->pBombs, pScene->pShips, (t_ptfVVF)BombCollide, (t_ptf)_SceneDummyFunction);
        }

        if(ContainerCard(pScene->pAliens) && ContainerCard(pScene->pShips))
            _SceneAlienCounterAttack(pScene);
		SceneDraw(pScene);
    	
		}
}

void SceneShipMoveToX(t_scene*pScene,int iX){
	if(ContainerCard(pScene->pShips)){
		ShipMove((t_ship*)ContainerGetback(pScene->pShips), iX);
	}
}

//0 = left, 1 = right
void SceneShipMoveJoystickDirection(t_scene*pScene, int iStickDir){
	if(iStickDir){
		ShipIncrementPosX((t_ship*)ContainerGetback(pScene->pShips), SCENE_SHIP_JOYSTICK_SPEED);
		iShipPos+=SCENE_SHIP_JOYSTICK_SPEED;
	}
	else{
		ShipIncrementPosX((t_ship*)ContainerGetback(pScene->pShips), -SCENE_SHIP_JOYSTICK_SPEED);
		iShipPos-=SCENE_SHIP_JOYSTICK_SPEED;
	}
}

void SceneSpacebarAction(t_scene*pScene){
	if(ContainerCard(pScene->pShips)){
		SDL_Point poHotpoint = ShipGetHotpoint((t_ship*)ContainerGetback(pScene->pShips));
		if(ContainerCard(pScene->pRockets)<SCENE_ROCKET_CONCURRENT_LIMIT){
			ContainerPushback(
				pScene->pRockets,
				RocketNew(
						&(SDL_Rect){poHotpoint.x-(SCENE_ROCKET_WIDTH/2), poHotpoint.y, SCENE_ROCKET_WIDTH, SCENE_ROCKET_HEIGHT},
						&(SDL_Point){0,-SCENE_ROCKET_SPEED}));
				SoundPlayShipShootingSound(pScene->pSound);
		}
	}
			//Here I set the speed as a negative value because the rocket needs to go UP
}

//-------------------- PRIVATE FUNCTIONS --------------------------/
void _SceneScore(){
	iScore+=50;
}

void _SceneChangeScore(t_scene*pScene){
	if(ContainerCard(pScene->pTexts)==2)TextDestroyTexture((t_text*)ContainerPopfront(pScene->pTexts));
	char buf[32];
	sprintf(buf,"Score %.5d", iScore);

	ContainerPushback(
		pScene->pTexts, 
		TextNew(
			&(SDL_Point){pScene->rSceneArea.x, SCENE_SCORE_PADDING},
			buf
			)
		);
}

void _SceneAlienCounterAttack(t_scene*pScene){
	if((rand()%101<SCENE_ALIEN_ATTACK_PERCENT) && (ContainerCard(pScene->pBombs)<SCENE_BOMB_CONCURRENT_LIMIT)){
		SDL_Point shipHotpoint = ShipGetHotpoint((t_ship*)ContainerGetback(pScene->pShips));
		SDL_Point alienBottomPoint = AlienGetBottomPoint((t_alien*)ContainerGetback(pScene->pAliens));
		SDL_Point alienSidePoint = AlienGetBottomPoint((t_alien*)ContainerGetfront(pScene->pAliens));
		if(ContainerCard(pScene->pAliens) && (shipHotpoint.x<alienBottomPoint.x) && (shipHotpoint.x>alienSidePoint.x))ContainerPushback(
				pScene->pBombs,
				BombNew(
						&(SDL_Rect){shipHotpoint.x-(SCENE_BOMB_WIDTH/2), alienBottomPoint.y, SCENE_BOMB_WIDTH, SCENE_BOMB_HEIGHT},
						&(SDL_Point){0,SCENE_BOMB_SPEED}));
		}
}

void _SceneDummyFunction(){
	return;
}

int SceneGetGameareaWidth(t_scene* pScene){
	return pScene->rSceneArea.w;
}

int SceneGetGamestate(t_scene*pScene){
    if (pScene->uGameState!=ST_SCENE_GAME_PLAYING) return 1;
    return 0;
}
