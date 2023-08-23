#include <lpc17xx.h>
#include <stdlib.h>
#include <stdio.h>
#include "GLCD.h"
#include "stdio.h"
#include "spece.h"
#include <cmsis_os2.h>
#include <os_tick.h>
#include "gameLogic.h"
	 
// declaring actors
actor* enemy1;
actor* enemy2;
actor* enemy3;
actor* win;
actor* player;

osThreadId_t animateID;

int main()
{
	//set up the main actors in the game
	initializeActors();

	//call this function to ensure that all of the internal settings of the LPC are correct
	SystemInit();
	printf("Test String");

	//Initialize the LCD screen
	GLCD_Init();
	GLCD_Clear(Black);
	//sets the color of any pixels we directly write to the screen, not just text
	GLCD_SetTextColor(Green);

	printPlayer(player);

	//initialize the kernel so that we can create threads
	osKernelInitialize();
	
	//create a new thread for animate and store its ID so that we can shut it down at the endgame
	animateID = osThreadNew(animate,NULL,NULL);
	
	//thread for reading push button
	osThreadNew(JUMP,NULL,NULL);
	
	//thread for reading player input
	osThreadNew(readPlayerInput,NULL, NULL);
	
	//thread that checks for collision
	osThreadNew(checkcollision,NULL,NULL);
	
	//thread for counting lives
	osThreadNew(LIVES_COUNT,NULL,NULL);
	
	//launch the kernel, which simultaneously starts all threads we have created
	osKernelStart();
	
	//Theoretically we will only ever entire this loop if something goes wrong in the kernel
	while(1){};
}
