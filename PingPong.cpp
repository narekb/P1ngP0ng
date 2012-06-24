#include "SDL/SDL.h"
#include <string>
#include <iostream>
using namespace std;
SDL_Surface *screen = NULL;
SDL_Surface *pad1= NULL;
SDL_Surface *pad2= NULL;
SDL_Surface *temp_pad1=NULL;
SDL_Surface *temp_pad2=NULL;
SDL_Surface *temp_ball=NULL;
SDL_Surface *ball=NULL;
SDL_Surface *temp_wall1=NULL;
SDL_Surface *temp_wall2=NULL;
SDL_Surface *wall1=NULL;
SDL_Surface *wall2=NULL;
SDL_Rect rect1;
SDL_Rect rect2;
SDL_Rect rect_ball;
SDL_Rect rect_wall1;
SDL_Rect rect_wall2;
int score1=0,score2=0; //Points of each player
int xVel=1, yVel=1; //Horizontal and vertical velocities of the ball
bool init()
{
	if(SDL_Init(SDL_INIT_EVERYTHING)<0)
	{
		cout<<"Failed to initialize SDL: "<<SDL_GetError()<<endl;
		exit(1);
	};
	screen=SDL_SetVideoMode(800, 600, 32, SDL_HWSURFACE | SDL_DOUBLEBUF);
	
	if(screen==NULL)
	{
		cout<<"Failed to set 800x600 32b mode "<<SDL_GetError()<<endl;
		exit(1);
	}
	SDL_WM_SetCaption("P1ngP0ng by N1tr0g3n", NULL);
	temp_pad1=SDL_LoadBMP("pad1.bmp");
	pad1=SDL_DisplayFormat(temp_pad1);
	SDL_FreeSurface(temp_pad1);
	if(!pad1)
	{
		cout<<"Failed to create Player 1 "<<SDL_GetError()<<endl;
		exit(1);
	}
	temp_pad2=SDL_LoadBMP("pad2.bmp");
	pad2=SDL_DisplayFormat(temp_pad2);
	SDL_FreeSurface(temp_pad2);
	if(!pad2)
	{
		cout<<"Failed to create Player 2 "<<SDL_GetError()<<endl;
		exit(1);
	}
	temp_ball=SDL_LoadBMP("ball.bmp");
	ball=SDL_DisplayFormat(temp_ball);
	SDL_FreeSurface(temp_ball);
	if(!ball)
	{
		cout<<"Failed to create ball "<<SDL_GetError()<<endl;
		exit(1);
	}
	temp_wall1=SDL_LoadBMP("wall.bmp");
	wall1=SDL_DisplayFormat(temp_wall1);
	if(!wall1)
	{
		cout<<"Failed to create top wall "<<SDL_GetError()<<endl;
		exit(1);
	}
	temp_wall2=SDL_LoadBMP("wall.bmp");
	wall2=SDL_DisplayFormat(temp_wall2);
	if(!wall2)
	{
		cout<<"Failed to create bottom wall "<<SDL_GetError()<<endl;
		exit(1);
	}
	rect1.x=10;
	rect1.y=10;
	rect2.x=770;
	rect2.y=10;
	rect_wall1.x=0;
	rect_wall1.y=0;
	rect_wall2.x=0;
	rect_wall2.y=590;
	rect_ball.x=400;
	rect_ball.y=300;
	SDL_EnableKeyRepeat(25,20);
	return true;
}

void draw()
{
	Uint32 color;
	color=SDL_MapRGB(screen->format,0 ,0, 0);
	SDL_FillRect(screen,NULL,color);
	SDL_BlitSurface(pad1, NULL, screen, &rect1);
	SDL_BlitSurface(pad2, NULL, screen, &rect2);
	SDL_BlitSurface(ball, NULL,screen, &rect_ball);
	SDL_BlitSurface(wall1,NULL,screen, &rect_wall1);
	SDL_BlitSurface(wall2,NULL,screen, &rect_wall2);
	SDL_Flip(screen);
	SDL_Delay(1);
}
bool collide(SDL_Rect A, SDL_Rect B)
{	
	//The sides of rectangles
	int leftA, leftB;
	int rightA, rightB;
	int topA, topB;
	int bottomA, bottomB;
	//Calculate sides of rect A
	leftA=A.x;
	rightA=A.x+A.w;
	topA=A.y;
	bottomA=A.y+A.h;
	
	leftB = B.x;
	rightB = B.x + B.w;
	topB = B.y;
	bottomB = B.y + B.h;
   //If any of the sides from A are outside of B
    if( bottomA <= topB )
    {
        return false;
    }
    
    if( topA >= bottomB )
    {
        return false;
    }
    
    if( rightA <= leftB )
    {
        return false;
    }
    
    if( leftA >= rightB )
    {
        return false;
    }
    //If none of the sides from A are outside B
    return true;
}
void score(int winner)
{
	if(winner==1)
	{
		score1++;
		cout<<"Player 1 earned 1 point!\n";
		rect1.x=10;
		rect1.y=10;
		rect2.x=770;
		rect2.y=10;
		rect_wall1.x=0;
		rect_wall1.y=0;         //Reset everything to default values and add score to winner
		rect_wall2.x=0;
		rect_wall2.y=590;
		rect_ball.x=400;
		rect_ball.y=300; 
		xVel=-1*xVel;
		yVel=-1*yVel;
	}
	if(winner==2)
	{
		score2++;
		cout<<"Player 2 earned 1 point!\n";
		rect1.x=10;
		rect1.y=10;
		rect2.x=770;
		rect2.y=10;
		rect_wall1.x=0;
		rect_wall1.y=0;         //Reset everything to default values and add score to winner
		rect_wall2.x=0;
		rect_wall2.y=590;
		rect_ball.x=400;
		rect_ball.y=300; 
		xVel=-1*xVel;
		yVel=-1*yVel;
	}
}
int main()
{
	int done=0;
	int fullscreen=0; //Just for checking if it's fullscreen or windowed
	Uint8 *keys;
	init(); //Call the initialization function
	cout<<"Welcome to P1ngP1ng! Player 1 - use W and X keys to move your pad.\nPlayer 2 - you use up and down arrow keys to move yours.\nPress F for fullscreen mode and R to reset the game. Have a nice time!\n";
	while(!done)	//Game loop
	{
		rect_ball.x+=xVel;
		rect_ball.y+=yVel;
		if(rect_ball.y>570 || rect_ball.y<10)
			yVel=-1*yVel;			
		if(collide(rect_ball, rect1)==true || collide(rect_ball, rect2)==true)
			xVel=-1*xVel;
		if(rect_ball.x < 10)
			score(2);
		if(rect_ball.x > 770)
			score(1);
		SDL_Event event;
		while(SDL_PollEvent(&event))
		{
			switch(event.type)
			{
				case SDL_KEYDOWN:
					keys=SDL_GetKeyState(NULL);
					if(keys[SDLK_w])
						if(rect1.y>10)
							rect1.y-=10;
					if(keys[SDLK_x])
						if(rect1.y<490)
							rect1.y+=10;
					if(keys[SDLK_UP])
						if(rect2.y>10)
							rect2.y-=10;
					if(keys[SDLK_DOWN])
						if(rect2.y<490)
							rect2.y+=10;
					if(keys[SDLK_f]) //Switch to fullscreen
					{
						if(fullscreen==0)
						{
							screen=SDL_SetVideoMode(800, 600, 32, SDL_HWSURFACE | SDL_DOUBLEBUF | SDL_FULLSCREEN);
							fullscreen=1;
							SDL_Delay(3000);
						}
						else if(fullscreen=1)
						{
							screen=SDL_SetVideoMode(800, 600, 32, SDL_HWSURFACE | SDL_DOUBLEBUF);
							fullscreen=0;
							SDL_Delay(3000);
						}
					}
					if(keys[SDLK_r]) //Restart the game
					{
						rect1.x=10;
						rect1.y=10;
						rect2.x=770;
						rect2.y=10;
						rect_wall1.x=0;
						rect_wall1.y=0;         //Reset everything to default values and add score to winner
						rect_wall2.x=0;
						rect_wall2.y=590;
						rect_ball.x=400;
						rect_ball.y=300; 
						xVel=-1*xVel;
						yVel=-1*yVel;
					}
					if(keys[SDLK_ESCAPE])
					{
						if(score1>score2)
							cout<<"Congratualtions, Player 1, you won this game!\n";
						if(score1<score2)
							cout<<"Congratualtions, Player 2, you won this game!\n";
						if(score1==score2)
							cout<<"Draw game!\n";
						cout<<"Thanks for playing P1ngP0ng, please report any bugs to nitrogen@fedoraproject.org\n";
					done=1;
					}
					break;
				case SDL_QUIT:
					if(score1>score2)
						cout<<"Congratualtions, Player 1, you won this game!\n";
					if(score1<score2)
						cout<<"Congratualtions, Player 2, you won this game!\n";
					if(score1==score2)
						cout<<"Draw game!\n";
					cout<<"Thanks for playing P1ngP0ng, please report any bugs to nitrogen@fedoraproject.org\n";	
					done=1;
				default:
					break;
			}
		}
		draw();
	}
SDL_FreeSurface(screen);
SDL_Quit();
return 0;
}
