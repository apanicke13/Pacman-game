#include "spece.h"

void drawBigPixelAt(int x, int y, int scale)
{
	int i;
	int j;
	for(i = 0; i < scale; i++)
	{
		for(j = 0; j < scale;j++)
			GLCD_PutPixel(x+i,y+j);
	}
}


void printSpriteAt(int x, int y,char* spriteBMP)
{
	//print the sprite left to right.
	//x is the vertical component and y is the horizontal
	//given the orientation of the screen, joystick, and button
	int i;
	int j;
	int spriteIndex = SPRITE_ROWS-1;
	int spriteShift = 0;
	
	for(i = (SPRITE_ROWS-1)*SPRITE_SCALE; i >= 0; i-=SPRITE_SCALE)
	{
		spriteShift = 0;
		for(j = 0; j < SPRITE_COLS*SPRITE_SCALE; j+=SPRITE_SCALE)
		{
			if((spriteBMP[spriteIndex] >> spriteShift)&1)
			{
				drawBigPixelAt(x+((SPRITE_ROWS-1)*SPRITE_SCALE-i),y+j,SPRITE_SCALE);
			}
			spriteShift++; //This is the column coordinate of the sprite's bitmap
		}
		spriteIndex--; //this is the row coordinate
	}
}

//updates ghost 1 position
void updateEnemy1Position(actor* act)
{
	if(act->dir == 1)
	{
		if((act->horizontalPosition + SPRITE_COLS*SPRITE_SCALE + act->speed) < HORIZONTAL_LIM)
			act->horizontalPosition += act->speed;
		else
		{
			act->dir = -1;
			if((act->verticalPosition - SPRITE_ROWS*SPRITE_SCALE - act->speed )>0)
			{
				act->verticalPosition -= act->speed;
			}
		}
	}
	
	else
	{
		if((act->horizontalPosition - act->speed) > 0)
			act->horizontalPosition -= act->speed;
		else
		{
			act->dir = 1;
			if((act->verticalPosition - SPRITE_ROWS*SPRITE_SCALE - act->speed )>0)
			{
				act->verticalPosition -= act->speed;
			}
		}
	}
}

//updates ghost 2 position
void updateEnemy2Position(actor* act)
{
	if(act->dir == 1)
	{
		if((act->verticalPosition + SPRITE_COLS*SPRITE_SCALE + act->speed) < VERTICAL_LIM)
			act->verticalPosition += act->speed;
		else
		{
			act->dir = -1;
			if((act->horizontalPosition + SPRITE_ROWS*SPRITE_SCALE + act->speed )<VERTICAL_LIM)
			{
				act->horizontalPosition += act->speed;
			}
		}
	}
	else
	{
		if((act->verticalPosition - act->speed) > 0)
			act->verticalPosition -= act->speed;
		else
		{
			act->dir = 1;
			if((act->horizontalPosition + SPRITE_ROWS*SPRITE_SCALE - act->speed )>0)
			{
				act->horizontalPosition += act->speed;
			}
		}
	}
}

//updates ghost 3 position
void updateEnemy3Position(actor* act)
{
	if(act->dir == 1)
	{
		if((act->verticalPosition + SPRITE_COLS*SPRITE_SCALE + act->speed) < VERTICAL_LIM)
			act->verticalPosition += act->speed;
		else
		{
			act->dir = -1;
			if((act->horizontalPosition - SPRITE_ROWS*SPRITE_SCALE - act->speed )>0)
			{
				act->horizontalPosition -= act->speed;
			}
		}
	}
	else
	{
		if((act->verticalPosition - act->speed) > 0)
			act->verticalPosition -= act->speed;
		else
		{
			act->dir = 1;
			if((act->horizontalPosition - SPRITE_ROWS*SPRITE_SCALE - act->speed )>0)
			{
				act->horizontalPosition -= act->speed;
			}
		}
	}
}

//printing ghost 1
void printEnemy1(actor* enemy1)
{
	GLCD_SetTextColor(Black);
	printSpriteAt(enemy1->verticalPosition,enemy1->horizontalPosition,enemy1->sprite);
	updateEnemy1Position(enemy1);
	GLCD_SetTextColor(Green);
	printSpriteAt(enemy1->verticalPosition,enemy1->horizontalPosition,enemy1->sprite);
}

//printing ghost 2
void printEnemy2(actor* enemy2)
{
	GLCD_SetTextColor(Black);
	printSpriteAt(enemy2->verticalPosition,enemy2->horizontalPosition,enemy2->sprite);
	updateEnemy2Position(enemy2);
	GLCD_SetTextColor(Green);
	printSpriteAt(enemy2->verticalPosition,enemy2->horizontalPosition,enemy2->sprite);
}

//printing ghost 3
void printEnemy3(actor* enemy3)
{
	GLCD_SetTextColor(Black);
	printSpriteAt(enemy3->verticalPosition,enemy3->horizontalPosition,enemy3->sprite);
	updateEnemy3Position(enemy3);
	GLCD_SetTextColor(Green);
	printSpriteAt(enemy3->verticalPosition,enemy3->horizontalPosition,enemy3->sprite);
}

//printing the Win location
void printWin(actor* winner)
{
	GLCD_SetTextColor(Black);
	printSpriteAt(winner->verticalPosition,winner->horizontalPosition,winner->sprite);
	GLCD_SetTextColor(Yellow);
	printSpriteAt(winner->verticalPosition,winner->horizontalPosition,winner->sprite);
}

//Printing the Player(PacMan)
void printPlayer(actor* play)
{
	GLCD_SetTextColor(Black);
	printSpriteAt(play->verticalPosition,play->horizontalPosition,play->sprite);
	
	//moves up the horizontal direction (y)
	if(play->dir == -1)  
		{
			if(play->horizontalPosition - play->speed > 0)
				play->horizontalPosition -= play->speed;
		}
		//moves down the horizontal direction (y) 
		else if(play->dir == 1)
		{
			if(play->horizontalPosition + play->speed + SPRITE_COLS*SPRITE_SCALE < HORIZONTAL_LIM)
				play->horizontalPosition += play->speed;
		}
		//moves up the vertical direction (x) 
		else if(play->dir == 2)
		{
			if(play->verticalPosition + play->speed + SPRITE_COLS*SPRITE_SCALE < VERTICAL_LIM)
				play->verticalPosition += play->speed;
		}
		//moves down the vertical direction (x) 
		else if(play->dir == 3)
		{
			if(play->verticalPosition - play->speed + SPRITE_COLS*SPRITE_SCALE < VERTICAL_LIM)
				play->verticalPosition -= play->speed;
		}
		
	GLCD_SetTextColor(Green);
	printSpriteAt(play->verticalPosition,play->horizontalPosition,play->sprite);
}

//printing player position with power up jump
void printPlayerpush(actor* play)
{
	GLCD_SetTextColor(Black);
	printSpriteAt(play->verticalPosition,play->horizontalPosition,play->sprite);
	
	//moves up the horizontal direction (y) with the power jump
	if(play->dir == -1)
		{
			if(play->horizontalPosition - play->push_speed > 0)
				play->horizontalPosition -= play->push_speed;
		}
		//moves down the horizontal direction (y) with the power jump
		else if(play->dir == 1)
		{
			if(play->horizontalPosition + play->push_speed + SPRITE_COLS*SPRITE_SCALE < HORIZONTAL_LIM)
				play->horizontalPosition += play->push_speed;
		}
		//moves up the vertical direction (x) with the power jump
		else if(play->dir == 2)
		{
			if(play->verticalPosition + play->push_speed + SPRITE_COLS*SPRITE_SCALE < VERTICAL_LIM)
				play->verticalPosition += play->push_speed;
		}
		//moves down the vertical direction (x) with the power jump
		else if(play->dir == 3)
		{
			if(play->verticalPosition - play->push_speed + SPRITE_COLS*SPRITE_SCALE < VERTICAL_LIM)
				play->verticalPosition -= play->push_speed;
		}

	GLCD_SetTextColor(Green);
	printSpriteAt(play->verticalPosition,play->horizontalPosition,play->sprite);
}

//checking for collision
bool checkCollision(actor* act1, actor* act2)
{
	//top corner act 1
	int x11;
	int y11;
	
	//top corner act 2
	int x21;
	int y21;
	
	//bottom corner act 1
	int x12;
	int y12;
	
	//bottom corner act 2
	int x22;
	int y22;
	
	x11 = act1->horizontalPosition;
	y11 = act1->verticalPosition;
	x12 = act1->horizontalPosition + SPRITE_COLS*SPRITE_SCALE;
	y12 = act1->verticalPosition + SPRITE_ROWS*SPRITE_SCALE;
	x21 = act2->horizontalPosition;
	y21 = act2->verticalPosition;
	x22 = act2->horizontalPosition + SPRITE_COLS*SPRITE_SCALE;
	y22 = act2->verticalPosition + SPRITE_ROWS*SPRITE_SCALE;
	
	return (x11<x22 && x12 > x21 && y11 < y22 && y12 > y21);
}
