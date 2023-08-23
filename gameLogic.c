#include "gameLogic.h"

extern actor* player;
extern actor* enemy1;
extern actor* enemy2;
extern actor* enemy3;
extern actor* win;

extern osThreadId_t animateID;

int lives=0;
//giving count a random starting value
int count=50;

//sprites for the enemy, player, and win location
char sprite[] = {0x3c,0x7e,0x66,0xe7,0xff,0xdb,0x00,0x00};
char winSprite[] = {0x3c,0x42,0x5a,0x5a,0x42,0x3c,0x00,0x00};
char playerSprite[] = {0x66,0x66,0x7e,0x7e,0x7e,0x3c,0x00,0x00};

const osMutexAttr_t Thread_Mutex_attr = {
	"myButtonMutex",
	0,
	NULL,
	0U
};
	
// initializing mutex globally 
osMutexId_t ex1;

// thread that reads player input
void readPlayerInput(void *args)
{
	while(1)
	{
		//checking for joystick input for movement
		if(!(LPC_GPIO1->FIOPIN & (1<<23))) 
		{
			player->dir = -1;
		}
		else if(!(LPC_GPIO1->FIOPIN & (1<<25)))
		{
			player->dir = 1;
		}
		else if(!(LPC_GPIO1->FIOPIN & (1<<24)))
		{
			player->dir = 2;
		}
		else if(!(LPC_GPIO1->FIOPIN & (1<<26)))
		{
			player->dir = 3;
		}
		else
			player->dir = 0;
		
		osThreadYield();
	}
}


void animate(void *args)
{

	while(1)
	{
		//printing ghosts
		printEnemy1(enemy1);
		printEnemy2(enemy2);
		printEnemy3(enemy3);
		
		//printing players
		if(player->dir != 0)
			printPlayer(player);
		
		//printing win location
		printWin(win);
			
		osDelay(10U);
	}
}

//thread to check for collision conditions
void checkcollision(void* args)
{
	
	while(1)
	{
	//acquiring mutex
		osMutexAcquire(ex1,osWaitForever);
		if(checkCollision(enemy1, player)||checkCollision(enemy2, player)||checkCollision(enemy3, player))
		{	
			lives+=1;
			//releasing mutex
			osMutexRelease(ex1);
		}
		else if(checkCollision(win, player))
		{	
			GLCD_SetTextColor(Black);
			//checking if player has reached win location
			GLCD_DisplayString(1,1,1," CONGRACTULATION !");	
			GLCD_DisplayString(2,1,1," YOU WON !");
			osThreadTerminate(animateID);
			osDelay(1000U);
		}
		
		if(count==0)
		{

			GLCD_SetTextColor(Black);
			//checking if player has lost the game	
			GLCD_DisplayString(1,1,1," YOU LOSE !");	
			GLCD_DisplayString(2,1,1," TRY AGAIN!");
			osThreadTerminate(animateID);
			osDelay(1000U);
		}
		osThreadYield();
	}
}

//thread for checking push button input
void JUMP(void* args)
{
	int powerups=1;
	unsigned int newpush=1;
	while(1)
	{
		
		unsigned int push= LPC_GPIO2->FIOPIN & (1<<10);
		osMutexAcquire(ex1,osWaitForever);
		
		//waiting till the push button is released
		if(newpush!=push){
			newpush=push;
			if(push==0)
			{
				if(powerups==1)
				{
					//calling the powerup movement function for the player
					printPlayerpush(player);
					powerups=0;
				}
			}
		}
		osMutexRelease(ex1);
	}	
}

//thread to count number of lives left for the player
void LIVES_COUNT(void* args)
{
	while(1)
	{
		//acquiring mutex
		osMutexAcquire(ex1,osWaitForever);
		
		if(lives==0)
		{
			//binary number of 7 is 111 which outputs as three lives
			count=7;
		}
		else if (lives>=1 && lives<=16)
		{
			//binary number of 3 is 11 which outputs as two lives
			count=3;
		}
		else if (lives>=16 && lives<=36)
		{
			//binary number of 1 is 1 which outputs as one life
			count=1;
		}
		else if (lives>=42)
		{
			//zero lives left
			count=0;
		}
		
		//outputing through the LEDs
		if (count & (1<<0))
				LPC_GPIO2->FIOSET |= 1<<6;
		else
					LPC_GPIO2->FIOCLR |= 1<<6;
		
		if (count & (1<<1))
				LPC_GPIO2->FIOSET |= 1<<5;
		else
					LPC_GPIO2->FIOCLR |= 1<<5;
		
		if (count & (1<<2))
				LPC_GPIO2->FIOSET |= 1<<4;
		else
					LPC_GPIO2->FIOCLR |= 1<<4;
		
		if (count & (1<<3))
				LPC_GPIO2->FIOSET |= 1<<3;
		else
					LPC_GPIO2->FIOCLR |= 1<<3;
		
		if (count & (1<<4))
				LPC_GPIO2->FIOSET |= 1<<2;
		else
					LPC_GPIO2->FIOCLR |= 1<<2;
		
		if (count & (1<<5))
				LPC_GPIO1->FIOSET |= 1U<<31;
		else
					LPC_GPIO1->FIOCLR |= 1U<<31;
		
		if (count & (1<<6))
				LPC_GPIO1->FIOSET |= 1<<29;
		else
					LPC_GPIO1->FIOCLR |= 1<<29;
		
		if (count & (1<<7))
				LPC_GPIO1->FIOSET |= 1<<28;
		else
					LPC_GPIO1->FIOCLR |= 1<<28;
		
		LPC_GPIO1->FIODIR |= 1<<28;
		LPC_GPIO1->FIODIR |= 1<<29;
		LPC_GPIO1->FIODIR |= 1U<<31;
		LPC_GPIO2->FIODIR |= 1<<2;
		LPC_GPIO2->FIODIR |= 1<<3;
		LPC_GPIO2->FIODIR |= 1<<4;
		LPC_GPIO2->FIODIR |= 1<<5;
		LPC_GPIO2->FIODIR |= 1<<6;
		
		//releasing the mutex
		osMutexRelease(ex1);
	}
}

//initialling actor attributes.
void initializeActors()
{

	enemy1 = malloc(sizeof(actor));
	enemy1->horizontalPosition = 0;
	enemy1->verticalPosition = 300;
	enemy1->speed = ENEMY_SPEED;
	enemy1->dir = 1;
	enemy1->sprite = sprite;
	
	enemy2 = malloc(sizeof(actor));
	enemy2->horizontalPosition = 0;
	enemy2->verticalPosition = 0;
	enemy2->speed = ENEMY_SPEED;
	enemy2->dir = 1;
	enemy2->sprite = sprite;
	
	enemy3 = malloc(sizeof(actor));
	enemy3->horizontalPosition = 215;
	enemy3->verticalPosition = 0;
	enemy3->speed = ENEMY_SPEED;
	enemy3->dir = 1;
	enemy3->sprite = sprite;

	player = malloc(sizeof(actor));
	player->horizontalPosition = 110;
	player->verticalPosition = 30;
	player->speed = PLAYER_SPEED;
	player->push_speed = PUSH_SPEED;
	player->dir = 0;
	player->sprite = playerSprite;
	
	win = malloc(sizeof(actor));
	win->horizontalPosition = 107;
	win->verticalPosition = 300;
	win->dir = 0;
	win->sprite = winSprite;
	
}
