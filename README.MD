# PSVita Space Invaders Clone

## Introduction
Welcome, this is a Space Invaders Clone I've been developing from time to time, when I thought about it. It is the port and upgrade of a PC Space Invaders port I had to write for a school project, it is made entirely in C with the SDL2 libraries (`SDL2, SDL2_ttf, SDL2_image, SDL2_mixer`)
So it is ported for the PS Vita now using the VitaSDK suite. Here are the controls :

```
DPad Left and Right - Ship movement
Left stick left and right - Same thing
Cross : Shoot
Start : Quit the game properly (destruction routines are run)
```

## Build
First off, you need [VitaSDK](https://vitasdk.org/), this is essential to compile this app. You could write a "Backwards Port" to get it running back on PC (Win/Linux/macOS) but it would be time consuming *(not really hard though)*.
To install it just follow the instruction this website is showing. You should then end up with VitaSDK and CMake installed.

Then you need to clone this repository and move using this command :
```bash
git clone https://www.github.com/Salim-ElBakraoui/Vita_SpaceInvaders.git && cd Vita_SpaceInvaders
```

To build the app you just need to run this command:

```bash
cmake . && make
```
Because the CMake file just creates a makefile... Mind blowing right ? Once make is done you should end up with a vpk file.

## Install
To install this app you need a softmodded PSVita running Henkaku or H-encore and VitaShell, or you can use the Vita3K emulator. 
It's just a matter of copying the VPK on the device and installing it all through VitaShell or simply dragging and dropping the VPK file on the emulator.