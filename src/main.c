/*
 ============================================================================
 Name        : main.c
 Author      : Thierry
 Version     : 1.0
 Copyright   : Lyc�e A. BOURDELLE
 Description : Space Invaders -Base-, Ansi-style
 ============================================================================
 */
#include <psp2/kernel/processmgr.h>
#include <psp2/audioout.h>
#include <stdio.h>
#include <stdlib.h>
#include <debugScreen.h>
#include "app.h"

#define VITA_SCREEN_WIDTH   960
#define VITA_SCREEN_HEIGHT  544

int _newlib_heap_size_user = 192 * 1024 * 1024;

int main(int argc, char*argv[]) {
	t_app* pApp=AppNew("Space Invaders -Base-", VITA_SCREEN_WIDTH, VITA_SCREEN_HEIGHT);
	int ret=AppRun(pApp);
    sceKernelExitProcess(0);
	return ret;
}
