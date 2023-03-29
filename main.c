#include "lpc17xx.h"
//#include "uart.h"
#include "stdio.h"
#include "GLCD.h"
#include <cmsis_os2.h>
#include "math.h"
#define NORMAL_MOVE 1
#define POWER_MOVE 2
#define GHOST_MOVE 2
#define x 7
#define captured 0

osMutexId_t ex1;

extern actor* PacMan;
extern actor* Ghost;
extern actor* Power_up;
extern osThreadId_t animateID;

//struct for coordinates
typedef struct 
{
    int x,y;
}coordinates;

//LED function to show lives remaining
int LIVES_COUNT()
{
	
	while(1){

        //assigning value as 7 as its binary value is 111 and we want the LEDs to show 3 lives left.

        //if statement to reduce number of lives if death(collision) has occurred.
        if(captured==1)
        {
            count++;
            captured = 0;
            if(count==1)
            {
                //assigning value as 3 as its binary value is 11 and we want the LEDs to show 2 lives left. 
                x=3;
            }
            else if(count==2)
            {
                //assigning value as 1 as its binary value is 1 and we want the LEDs to show 1 life left.
                x=1;
            }
        }

		if (x & (1<<0))
		{
            LPC_GPIO2->FIOSET |= 1<<6;
        }
		else
        {
			LPC_GPIO2->FIOCLR |= 1<<6;
        }

		
		if (x & (1<<1))
        {
            LPC_GPIO2->FIOSET |= 1<<5;
        }		
		else
		{
			LPC_GPIO2->FIOCLR |= 1<<5;
        }
		

		if (x & (1<<2))
		{
            LPC_GPIO2->FIOSET |= 1<<4;
        }
		else
		{
			LPC_GPIO2->FIOCLR |= 1<<4;
        }
		

		if (x & (1<<3))
		{
            LPC_GPIO2->FIOSET |= 1<<3;
        }
		else
		{
			LPC_GPIO2->FIOCLR |= 1<<3;
        }
		

		if (x & (1<<4))
		{
            LPC_GPIO2->FIOSET |= 1<<2;
        }
		else
		{
			LPC_GPIO2->FIOCLR |= 1<<2;
        }

		
		if (x & (1<<5))
		{
            LPC_GPIO1->FIOSET |= 1U<<31;
        }
		else
		{
			LPC_GPIO1->FIOCLR |= 1U<<31;
        }
		
		if (x & (1<<6))
		{
            LPC_GPIO1->FIOSET |= 1<<29;
        }
		else
		{
			LPC_GPIO1->FIOCLR |= 1<<29;
        }
		
		if (x & (1<<7))
		{
            LPC_GPIO1->FIOSET |= 1<<28;
        }
		else
		{
			LPC_GPIO1->FIOCLR |= 1<<28;
        }
		
        //setting the pins' direction to output
		LPC_GPIO1->FIODIR |= 1<<28;
		LPC_GPIO1->FIODIR |= 1<<29;
		LPC_GPIO1->FIODIR |= 1U<<31;
		LPC_GPIO2->FIODIR |= 1<<2;
		LPC_GPIO2->FIODIR |= 1<<3;
		LPC_GPIO2->FIODIR |= 1<<4;
		LPC_GPIO2->FIODIR |= 1<<5;
		LPC_GPIO2->FIODIR |= 1<<6;
	
		osMutexRelease(ex1);
	}
	
}

//LCD function to print the game map onto the LCD screen.
int GAME_MAP()
{
	while(1){
	
		//checking for input on the joystick and displaying output on LCD.
		GLCD_DisplayString(1,1,1,"******************************");
        GLCD_DisplayString(2,1,1,"*                            *");
        GLCD_DisplayString(3,1,1,"*  *  *  ***********  *   *  *");
        GLCD_DisplayString(4,1,1,"*  *  *               *   *  *");
        GLCD_DisplayString(5,1,1,"*  *  *    ************   *  *");
        GLCD_DisplayString(6,1,1,"*  *  *               *   *  *");
        GLCD_DisplayString(7,1,1,"*  *  *  ***********  *   *  *");
        GLCD_DisplayString(8,1,1,"*                            *");
        GLCD_DisplayString(9,1,1,"******************************");
        GLCD_DisplayString(5,9,1,"c");
        GLCD_DisplayString(6,2,1,"W");
        GLCD_DisplayString(2,13,1,"P");
        GLCD_DisplayString(5,23,1,"G");

		osThreadYield();
	}
}

//joystick function to make pacman move.
int MOVEMENT()
{
	while(1){
	
	    int up =0;
		int right =0;
		int down = 0;
	    int left = 0;
		int joy = 0;
        //starting position of the PacMan
        int n_x=5;
        int n_y=9;

        //starting position for ghost
        int g_x=5;
        int g_y=23;
        int x=0;
        int y=0;
        int move=NORMAL_MOVE;
        int x1=0;
        int y1=0;

		
		while(1){
	
			//assigning Joystick's bit value to variable
			up = LPC_GPIO1->FIOPIN & (1<<23);
			right = LPC_GPIO1->FIOPIN & (1<<24);
			down = LPC_GPIO1->FIOPIN & (1<<25);
			left = LPC_GPIO1->FIOPIN & (1<<26);
			joy = LPC_GPIO1->FIOPIN & (1<<20);
			
            x=n_x;
            y=n_y;

			//checking for input on the joystick and displaying output on LCD.
			if(up==0)
			{
                GLCD_DisplayString(n_x,n_y,1," ");
                x+=move;
                //checking for win
                if(x==6 && y==2)
                {
                    GLCD_DisplayString(1,1,1,"YOU WIN !!!");
                }

                //movement to the top
                if(GLCD_DisplayString(x,y,1," "))
                {
                    GLCD_DisplayString(x,y,1,"c");
                }
                else
                {
                    GLCD_DisplayString(n_x,n_y,1,"C");
                }
                n_x=x;
            }

			else if(down==0)
			{
                GLCD_DisplayString(n_x,n_y,1," ");
                x-=move;
                //checking for win
                if(x==6 && y==2)
                {
                    GLCD_DisplayString(1,1,1,"YOU WIN !!!");
                }

                //movement to the bottom
                if(GLCD_DisplayString(x,y,1," "))
                {
                    GLCD_DisplayString(x,y,1,"c");
                }
                else
                {
                    GLCD_DisplayString(n_x,n_y,1,"C");
                }
                n_x=x;
            }
				
			else if(right==0)
			{
                GLCD_DisplayString(n_x,n_y,1," ");
                y+=move;
                //checking for win
                if(x==6 && y==2)
                {
                    GLCD_DisplayString(1,1,1,"YOU WIN !!!");
                }

                //movement to the right
                if(GLCD_DisplayString(x,y,1," "))
                {
                    GLCD_DisplayString(x,y,1,"c");
                }
                else
                {
                    GLCD_DisplayString(n_x,n_y,1,"C");
                }
                n_y=y;
            }
			
			else if(left==0)
			{
                GLCD_DisplayString(n_x,n_y,1," ");
                y-=move;
                //checking for win
                if(x==6 && y==2)
                {
                    GLCD_DisplayString(1,1,1,"YOU WIN !!!");
                }

                //movement to the left
                if(GLCD_DisplayString(x,y,1," "))
                {
                    GLCD_DisplayString(x,y,1,"c");
                }
                else
                {
                    GLCD_DisplayString(n_x,n_y,1,"C");
                }
                n_y=y;
            }			
			
			if(joy==0)
			{
                GAME_MAP();
            }

            //moving the ghost
            if(g_x!=n_x && g_y!=n_y)
            {
                x1=g_x;
                y1=g_y;
                if (n_x<g_x)
                {
                    GLCD_DisplayString(g_x,g_y,1," ");
                    g_x=+GHOST_MOVE;
                    if(GLCD_DisplayString(x1,y1,1," "))
                    {
                        GLCD_DisplayString(x1,y1,1,"G");
                    }
                    else
                    {
                        GLCD_DisplayString(g_x,g_y,1,"G");
                    }
                    g_x=x1;
                }

                else if (n_x>g_x)
                {
                    GLCD_DisplayString(g_x,g_y,1," ");
                    g_x=-GHOST_MOVE;
                    if(GLCD_DisplayString(x1,y1,1," "))
                    {
                        GLCD_DisplayString(x1,y1,1,"G");
                    }
                    else
                    {
                        GLCD_DisplayString(g_x,g_y,1,"G");
                    }
                    g_x=x1;
                }
                else if (n_y>g_y)
                {
                    GLCD_DisplayString(g_x,g_y,1," ");
                    g_y=+GHOST_MOVE;
                    if(GLCD_DisplayString(x1,y1,1," "))
                    {
                        GLCD_DisplayString(x1,y1,1,"G");
                    }
                    else
                    {
                        GLCD_DisplayString(g_x,g_y,1,"G");
                    }
                    g_y=y1;
                }
                else if (n_y<g_y)
                {
                    GLCD_DisplayString(g_x,g_y,1," ");
                    g_y=-GHOST_MOVE;
                    if(GLCD_DisplayString(x1,y1,1," "))
                    {
                        GLCD_DisplayString(x1,y1,1,"G");
                    }
                    else
                    {
                        GLCD_DisplayString(g_x,g_y,1,"G");
                    }
                    g_y=y1;
                }
                
            }
            else if (g_x==n_x && g_y==n_y)
            {
                GLCD_DisplayString(1,1,1,"NOOB! YOU LOSE");
                captured = 1;
            }
        }
		osThreadYield();

	}
}


//Function using push button to reset full game.
int RESET_GAME(){
	
	while(1){
		
		unsigned int push= LPC_GPIO2->FIOPIN & (1<<10);

		if(push==0)
        {	
			x=7;
            GAME_MAP();
            captured = 0;
		}

        osThreadYield();
	}
		
}
