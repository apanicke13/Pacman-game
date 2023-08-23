#ifndef GAMELOGIC
#define GAMELOGIC

#include <lpc17xx.h>
#include <stdlib.h>
#include <stdio.h>
#include "GLCD.h"
#include "spece.h"
#include <cmsis_os2.h>
#include <os_tick.h>

void readPlayerInput(void *args);
void animate(void *args);
void checkcollision(void* args);
void LIVES_COUNT(void* args);
void JUMP(void* args);
void initializeActors(void);

#endif
