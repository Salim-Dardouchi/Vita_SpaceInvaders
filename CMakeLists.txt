cmake_minimum_required(VERSION 2.8.12)

set(ENV{VITASDK} "/usr/local/vitasdk")
if(NOT DEFINED CMAKE_TOOLCHAIN_FILE)
  if(DEFINED ENV{VITASDK})
    set(CMAKE_TOOLCHAIN_FILE "$ENV{VITASDK}/share/vita.toolchain.cmake" CACHE PATH "toolchain file")
  else()
    message(FATAL_ERROR "Please define VITASDK to point to your SDK path!")
  endif()
endif()

project(SpaceInvadersCloneSDL)
include("${VITASDK}/share/vita.cmake" REQUIRED)

set(VITA_APP_NAME "Space Invaders Clone")
set(VITA_TITLEID  "SPAC21102")
set(VITA_VERSION  "01.00")

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=gnu11")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

include_directories(
  Container
  Defines
  Figures
  Font
  Scene
  Sound
  PSVDebug
)

add_executable(${PROJECT_NAME}
  src/main.c
  src/app.c
  Container/container.c
  Scene/scene.c
  Figures/alien.c
  Figures/ship.c
  Figures/bomb.c
  Figures/rocket.c
  Font/text.c
  Sound/sound.c
  PSVDebug/debugScreen.c
)

target_link_libraries(${PROJECT_NAME}
  SDL2
  SDL2_ttf
  SDL2_image
  SDL2_mixer
  freetype
  png
  jpeg
  webp
  pthread
  z
  FLAC
  mikmod
  mpg123
  vorbis
  ogg
  vorbisfile
  opus
  SceDisplay_stub
  SceCtrl_stub
  SceAudio_stub
  SceSysmodule_stub
  SceGxm_stub
  SceCommonDialog_stub
  SceAppMgr_stub
  SceTouch_stub
  SceHid_stub
  SceMotion_stub
  m
)

vita_create_self(${PROJECT_NAME}.self ${PROJECT_NAME})
vita_create_vpk(${PROJECT_NAME}.vpk ${VITA_TITLEID} ${PROJECT_NAME}.self
  VERSION ${VITA_VERSION}
  NAME ${VITA_APP_NAME}
  FILE sce_sys/icon0.png sce_sys/icon0.png
  FILE sce_sys/livearea/contents/bg.png sce_sys/livearea/contents/bg.png
  FILE sce_sys/livearea/contents/startup.png sce_sys/livearea/contents/startup.png
  FILE sce_sys/livearea/contents/template.xml sce_sys/livearea/contents/template.xml

  FILE Sound/Audio/alienKilled.wav                  Sound/Audio/alienKilled.wav
  FILE Sound/Audio/music.mp3                        Sound/Audio/music.mp3
  FILE Sound/Audio/shipKilled.wav                   Sound/Audio/shipKilled.wav
  FILE Sound/Audio/shoot.wav                        Sound/Audio/shoot.wav

  FILE Scene/Background/newspacehd.jpg              Scene/Background/newspacehd.jpg

  FILE Figures/Images/alien-transparent.png         Figures/Images/alien-transparent.png
  FILE Figures/Images/alien-transparent-test.png    Figures/Images/alien-transparent-test.png
  FILE Figures/Images/laserDepiction.png            Figures/Images/laserDepiction.png
  FILE Figures/Images/missile.png                   Figures/Images/missile.png
  FILE Figures/Images/missile-test.png              Figures/Images/missile-test.png
  FILE Figures/Images/spaceship.png                 Figures/Images/spaceship.png
  FILE Figures/Images/spaceship-test.png            Figures/Images/spaceship-test.png

  FILE Font/earlyGameboy.ttf                           Font/earlyGameboy.ttf

)
