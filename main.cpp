/*
	NAME: Steven Ortiz 
 	BOID PROJECT
 	The Boids project demonstrates the pattern of birds,v, based on three rules on the command terminal
 	Patterns are based on three simple rules. 
 	The first rule is to gather by using center of mass in the X and Y direction. 
 	The second rule is to keep the boids from having the same X and Y position meaning they do not overlap. A factor is used to repel them if they are close. 
 	The third rule is to have all boids with the same velocity 
 	ECGR 2103- Computer Utilization in c++  Final Project 
 	Template by. Dr. Ravindran @UNC-Charlotte 
 	Semester: Fall 2014 
 	last edit: spring 2018 to upload to Github 
 	Improvments needed are boid should be constained inside grid 
  
*/

//Libraries included to be used in this project

#include <iostream>     
#include <cstdlib>
#include <ctime>
#include <ncurses.h> //endCurses & StartCurses ** used to maniplulate the terminal 
#include <cmath>
#include <math.h>
#include <unistd.h> // sleep function 

#define NUM_BOIDS 10		 //defines that the number of boids 10

#define GRID_X 50			// define the size of the grid for rows 50

#define GRID_Y 60			//defines that size of the grid for columns 60

#define POS_MAX_X 50			//the initial position will not surpass this number 

#define POS_MAX_Y 60		   //the initial position will not surpass this number  

#define VEL_MAX_X 3				// the velocity will not exceed this number 

#define VEL_MAX_Y 3				//the velocity will not exceed this number 

#define NUM_ITER  12				// number of iterations  12

#define delta_T 1				// defines that the program will pause for one second between each loop

using namespace std;

class Velocity 					//declares class to hold velocity values
{
	public:
		double vX; 			// vX holds value for velocity in the x direction
		double vY; 			//vY holds value for velocity in the Y direction
};

class Position 					//declares class to hold position values
{
	public:
		double X; 			//X holds value for position in the x direction
		double Y; 			//Y holds value for position in the y direction
};

class Boid 					//declares class to hold the Boid voids 
{
	public:
		Velocity vel; 			//creates an object called vel of class Velocity for each Boid object
		Position pos; 			//creates an object called pos of class Position for each Boid object
};

void startCurses();				

void endCurses(); 				



void initialize_boids(Boid b[]); 		

void clear_grid(char grid[][GRID_Y]); 		

void display_boids(Boid b[], char grid[][GRID_Y]); 

void move_boids(Boid b[]); 				



bool curses_started = false; 			



int main()
{
	//Object b of type Boid
	Boid b[NUM_BOIDS];			// this b array holds all the information for type BOID b[] array 
	char grid[GRID_X][GRID_Y]; // Multidimensional array 
						
	
	srand(time(0)); // for the time function to be used randomizing values 

	
	startCurses(); 				
	initialize_boids(b); 			
	clear_grid(grid);			
	clear();
	
	display_boids(b,grid);			
	refresh();
	cout<<"************************Initial Boid positions********************"<<endl; 
	sleep(delta_T);				
	
	for (int i = 0; i < NUM_ITER; i++)		
	{
		clear_grid(grid);			
		move_boids(b);				 
		clear();
		display_boids(b,grid);			
		refresh();
		cout<<endl<<"End of iteration"<<i<<endl;
		sleep(delta_T);				
	}
	
	endCurses();					
	
	
return 0;
}

/*****************************************************************************
 * the X & Y positions are intiatied based the predefined POS and VEL max and min
 * the X & Y veclocitys are intiated 
 * ***************************************************************************/
void initialize_boids(Boid b[]) 				//Gives NUM_BOIDS random double values  for X, Y, VX, VY
{ 
	
	for(int i=0; i < NUM_BOIDS; i++)
	{  //the +1 is to be inclusive 
		b[i].pos.X = rand()%POS_MAX_X+1;		// create a random start for the x position 
      b[i].pos.Y = rand()%POS_MAX_Y+1;		// create a random start for the y position 
		b[i].vel.vX= rand()%VEL_MAX_X+1; 			 
		b[i].vel.vY= rand()%VEL_MAX_Y+1;			 
	}
}
/*****************************************************************************
 * The grid is filled with char spaces ' '
 *  ***************************************************************************/
void clear_grid(char grid[GRID_X][GRID_Y]) 				// this first clears the grid by having a blank space. 		
{
	for( int i=0;i < GRID_X ; i++)
		for( int j=0; j< GRID_Y; j++)
			 grid[i][j]=' ';		
}


/*****************************************************************************
 * Will place  a char type 'v' on the grid from properties of type Boid b vector
 * will create the grid on the terminal with mvaddch functions  
 *  ***************************************************************************/
void display_boids(Boid b[], char grid[][GRID_Y]) 			
{
	// the positions of the boids are placed on the Grid 
	for( int i=0; i < NUM_BOIDS;i++)	
		grid[static_cast<int>(b[i].pos.X)][static_cast<int>(b[i].pos.Y)]= '0'; 	//the position of X and Y are mapped on they type char grid to display 'v' 

	// creates the grid on the terminal 
    for (int i = 0; i < GRID_X; i++){
        for (int j = 0; j < GRID_Y; j++) { 	
            mvaddch(i,j, grid[i][j]);
            }  
       mvaddch(i,GRID_Y,'\n');         //
    }
}

/*****************************************************************************
 * 
 * Rule 1 gathers
 * 	caculate the avegrage position of the X and Y direction 
 * 	Adjust current X & Y position based on CenterX & Center Y respectively
 *  Rule 2 separates
 * 	if distance of the X & Y position are less than 1 unit away move each boid 
 * 	1 unit away from each other
 *  Rule 3 moves together 
 * 	Velocity is adjusted once each boid is closer togehter 
 *  ***************************************************************************/
void move_boids(Boid b[]) 				// This function includes Rules 1,2,3  
{
	//RULE 1
	// FOR X 
	double totalMX = 0;
	double centerX;


	for(int i=0; i < NUM_BOIDS; i++)
		totalMX += b[i].pos.X;
	//avg of the X position
	centerX =totalMX/(NUM_BOIDS);

	 
	//  X pos of current boid = X pos of current boid) + 20% *( Centerx- X pos ofcurrent boid
	for(int k=0; k < NUM_BOIDS;k++)
		b[k].pos.X = b[k].pos.X +((centerX-b[k].pos.X)*.2);	// this will set the new position X
	

   // FOR Y
	double totalMY=0;
	double centerY;

	for(int i=0; i < NUM_BOIDS; i++)
		{totalMY += b[i].pos.Y;}
	// avg  of the Y position 
	centerY =totalMY/(NUM_BOIDS);

	// y pos of current boid = y pos of current boid) + 20% *( CenterY - Y pos ofcurrent boid
	for(int l=0; l < NUM_BOIDS;l++)	
		{b[l].pos.Y = b[l].pos.Y +((centerY-b[l].pos.Y)*.2);}		// this will set the new position Y


	//RULE 2	

	for(int c=0; c < NUM_BOIDS; c++)
		for( int d=0;d < NUM_BOIDS; d++)				
			if((abs(b[c].pos.X - b[d].pos.X) < 1) && (abs(b[c].pos.Y -b[d].pos.Y) < 1) )
					{
					b[c].pos.X = b[c].pos.X + 1;
					b[c].pos.Y = b[c].pos.Y + 1;
					b[d].pos.X = b[d].pos.X -1;
					b[d].pos.Y = b[d].pos.Y - 1;
					}

						
	//RULE 3 
		
	//For velocity vX
	double totalVX =0;
	double centerVX;


	for(int i = 0; i < NUM_BOIDS; i++)
		{totalVX +=b[i].vel.vX;} 					//this is the total velocity of the NUM_BOIDS

	centerVX = totalVX/(NUM_BOIDS);			   //Avg velocty of all NUM_BOIDS 

// velocity of ecah NUM_BOID is adjusted 
	for ( int p =0; p < NUM_BOIDS; p++)
		{b[p].vel.vX += ((centerVX-b[p].vel.vX)*.125);}
	 
	 
	for(int r =0; r < NUM_BOIDS; r++)
		b[r].pos.X = b[r].vel.vX + b[r].pos.X; 				// this adds the new vX to the X position  

	//For velocity vY 
	
	double totalVY =0;
	double centerVY;
	
	for(int i = 0; i < NUM_BOIDS; i++)
		{totalVY +=b[i].vel.vY;}				
		centerVY = totalVY/(NUM_BOIDS);
	for ( int p =0; p < NUM_BOIDS; p++)
		b[p].vel.vY += ((centerVY-b[p].vel.vY)*.125);
 
	for(int o =0; o < NUM_BOIDS; o++)
		b[o].pos.Y = b[o].vel.vY + b[o].pos.Y; 				// this adds the new vY to the Y position 

}



void endCurses()							
{
	if (curses_started && !isendwin())			
	endwin();
					
}

void startCurses()						
{
	if (curses_started)					
	{
		refresh();					
	}
	else
	{
		initscr();
		cbreak();
		noecho();
		intrflush(stdscr, false);
		keypad(stdscr, true);
		atexit(endCurses);
		curses_started = true; 				
	}
}
