#ifndef SPECE
#define SPECE

#include "GLCD.h"
#include <stdbool.h>
#include <math.h>
#include <stdlib.h>

//sprites are 6x8 bitmaps, scaled by SPRITE_SCALE
#define SPRITE_ROWS 6
#define SPRITE_COLS 8
#define SPRITE_SCALE 3

//size of the screen
#define VERTICAL_LIM 330
#define HORIZONTAL_LIM 240

//speeds
#define PLAYER_SPEED 10
#define PUSH_SPEED 50
#define ENEMY_SPEED 25

typedef struct actor_struct{
	int verticalPosition;
	int horizontalPosition;
	char* sprite;
	int dir; 
	int speed;
	//number of pixels to move each time
	int push_speed;
}actor;

//Prints a sprite starting at that sprite's top-left corner
void printSpriteAt(int x, int y,char* spriteBMP);

void drawBigPixelAt(int x, int y, int scale);

//updates x and y position of the enemy
void updateEnemyPosition(actor* act);

//prints an enemy
void printEnemy(actor* enemy);

//prints the win location
void printWin(actor* win);

//prints the player
void printPlayer(actor* play);

//determines if any collision occurs
bool checkCollision(actor* act1, actor* act2);

#endif
