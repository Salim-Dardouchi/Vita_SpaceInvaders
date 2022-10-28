/*
 * scene.h
 *
 *  Created on: 5 avr. 2021
 *      Author: Thierry
 */

#define VITA_PORT

#define SCENE_FONT_PATH              "app0:/Font/earlyGameboy.ttf"
#define	SCENE_SHIP_IMG_PATH			 "app0:/Figures/Images/spaceship-test.png"
#define SCENE_ALIEN_IMG_PATH		 "app0:/Figures/Images/alien-transparent-test.png"
#define SCENE_BACKGROUND_PATH		 "app0:/Scene/Background/newspacehd.jpg"
#define SCENE_ROCKET_IMG_PATH		 "app0:/Figures/Images/missile-test.png"
#define SCENE_BOMB_IMG_PATH          "app0:/Figures/Images/laserDepiction.png"

#define SCENE_SCORE_PADDING				(4)
#define SCENE_SCORE_FONT_SIZE			(24)
#define SCENE_MSG_FONT_SIZE				(72)

#define SCENE_SHIP_PLAYABLE_SIZE		(32)
#define SCENE_SHIP_REDUCED_SIZE			(24)
#define SCENE_SHIP_REDUCED_PAD			(4)

#define SCENE_SHIP_LIVES				(4)
#define SCENE_SHIP_SPEED				(0)
#define SCENE_SHIP_JOYSTICK_SPEED		(4)

#define SCENE_ALIEN_SIZE				(32)
#define SCENE_ALIEN_SPACING				(4)
#define	SCENE_ALIEN_PADDING_VRT			(24)
#define SCENE_ALIEN_PADDING_HRZ			(32)

#define SCENE_ALIEN_SPEED_HRZ			(3)
#define SCENE_ALIEN_SPEED_VRT			(20)

#define SCENE_ALIEN_ROWS				(5)
#define	SCENE_ALIEN_COLS				(10)

#define SCENE_ALIEN_ATTACK_PERCENT		(10)

#define SCENE_BACKGROUND_RES_X			(960)
#define SCENE_BACKGROUND_RES_Y			(544)

#define SCENE_ROCKET_SPEED				(10)
#define	SCENE_ROCKET_WIDTH				(7)
#define SCENE_ROCKET_HEIGHT				(28)

#define SCENE_ROCKET_CONCURRENT_LIMIT	(1)

#define SCENE_BOMB_SPEED				(7)
#define SCENE_BOMB_WIDTH				(5)
#define SCENE_BOMB_HEIGHT				(25)

#define SCENE_BOMB_CONCURRENT_LIMIT		(1)



typedef struct s_scene t_scene;
t_scene*SceneNew(SDL_Rect*pSceneArea,SDL_Renderer*pRenderer,SDL_Color*pColorBkgnd);
t_scene*SceneDel(t_scene*pScene);
void SceneDraw(t_scene*pScene);
void SceneAnimate(t_scene*pScene);
void SceneShipMoveToX(t_scene*pScene,int iX);
void SceneSpacebarAction(t_scene*pScene);
int SceneGetGamestate(t_scene*pScene);
void SceneShipMoveJoystickDirection(t_scene*pScene, int iStickDir);
int SceneGetGameareaWidth(t_scene* pScene);