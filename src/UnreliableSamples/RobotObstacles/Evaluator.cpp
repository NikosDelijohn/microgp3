/*
This evaluator simulatetes a simple robot that can rotate and move straight avoiding obstacles

  This file is part of MicroGP v3 (ugp3) 
  Copyright (c) 2006-2016 Giovanni Squillero   

  Written by
    Alessio Moscatello 	<alessiomosc@yahoo.it>
  Extended by
    Alberto Tonda	<alberto.tonda@gmail.com>


*/


#include <iostream>
#include <cmath>
#include <fstream>
#include <string>
using namespace std;


// arena width/height 
#define ARENA 100

// final goal
#define X_GOAL_ 6
#define Y_GOAL_ 54

// number of walls
#define N_WALLS 2

// wall type 1
/* 
* * * * * * * * * * * 

	A * B
	*   *	
	*   *
	*   *
* * * * C   D * * * *
*/

#define X1 80
#define X2 82
#define Y1_1 -10
#define Y2_1 90

// wall type 2
/* 
* * * *	A   B * * * * 
	*   *	
	*   *
	*   *
	C * D

* * * * * * * * * * *
*/

#define Y1_2 100
#define Y2_2 10




bool test_intersection(float x11, float y11,float x12, float y12,float x21, float y21,float x22, float y22,float *X,float *Y);




int main (int argc, char** argv)
{
  
  float x,y,x_new,y_new,x_goal, y_goal;
  float X_intersez,Y_intersez;
  float y1_wall[2], y2_wall[2];
  float max_distance = 0;

  // initializing coords of the goal (it's shifted onwards)
  x_goal = X_GOAL_ + N_WALLS * ARENA;
  y_goal = Y_GOAL_;

  #ifdef DEBUG
  	cout << "Goal is (" << x_goal << "," << y_goal << ") , since the arena is " << N_WALLS * ARENA << "x100"<< endl;
  #endif

  // initializing max value of distance from goal (used later to compute fitness)
  max_distance = sqrt( pow(x_goal,2) + pow(y_goal,2) );

  #ifdef DEBUG
  	cout << "Minimum fitness value is " << max_distance << endl;
  #endif

  // initialazing coords of the walls
  y1_wall[0] = Y1_1;
  y2_wall[0] = Y2_1;
  y1_wall[1] = Y1_2;
  y2_wall[1] = Y2_2;
  
  
  ofstream path; 					//the path of a single individual
  ofstream ofile("fitness.output");			//output fitness that will be read by ugp
  ofstream myfitness("whole_fitness.txt",ios::app); 	//the fitness values of all individuals evaluated are stored 
							//in this file (useful for plotting graphs)
  
  string s;

  float direction, rotate_by;
  float movement;
  
  bool limits_surpassed, wall_crossed, simulate = false;
  
  float COMP1,COMP2;
  
  long int step_number;
  
  int i;
  
  
  s.erase();
  s.append(argv[1]);
  
  if(s=="simulate"){
    i=2;
    simulate=true;
    cout << "\n Evaluating path of the individual " << argv[2] << endl;
    
    s.erase();
    s.append("coordinates.txt");
    
    path.open(s.c_str(),fstream::trunc);
    path << "0.0 0.0" << endl;
  }
  else
    i=1;
  
  
  
  
  for(; i < argc; i++) //for each filename
  {
  //reset variables
  x=0;
  y=0;
  direction=0;
  movement=0;  
  step_number=0;
  
  
  
  limits_surpassed = false;
  wall_crossed = false;

  ifstream file(argv[i]);
  
  while(file.eof() == false)
  {
    s = '\0'; 
    file >> s;
    
    //parse the instructions of the individual generated by ugp

    if(s == "move")
    {
      
      
      file >> movement;
      
      if(movement!=0)
	step_number++;
      
      
      #ifdef DEBUG
      	cout <<endl<< "Position: (" << x <<"," << y <<")"  << endl;
      #endif
      
      //new position
      x_new = x + movement * cos(direction);
      y_new = y + movement * sin(direction); 
      
      #ifdef DEBUG
	cout <<endl<< "Start position: (" << x <<"," << y <<")"  << endl;
	cout <<endl<< "End position: (" << x_new <<"," << y_new <<")\n"  << endl;
      #endif
      
      
      
	// for each wall
	for(int iWall = 0; iWall < N_WALLS; iWall++)
	{
		int i = iWall % 2;
		int x1_wall = X1 + ARENA * iWall;
		int x2_wall = X2 + ARENA * iWall;
      
		//test if the segment representing the movement intersects the segments representing the walls
		
		if(test_intersection(x, y, x_new, y_new, x1_wall, y1_wall[i], x1_wall, y2_wall[i], &X_intersez, &Y_intersez)){
			
			#ifdef DEBUG
				cout << "\nWarning: wall crossed at: "<< X_intersez << " _ "<< Y_intersez << endl;
			#endif
			wall_crossed = true;
			
			
		}
		
		
		if(test_intersection(x, y, x_new, y_new, x1_wall, y2_wall[i], x2_wall, y2_wall[i], &X_intersez, &Y_intersez))
		{
			
			#ifdef DEBUG
				cout << "\nWarning: wall crossed at: "<< X_intersez << " _ "<< Y_intersez << endl;
			#endif
			wall_crossed=true;
			
			
		}
		
		
		
		if(test_intersection(x, y, x_new, y_new, x2_wall, y2_wall[i], x2_wall, y1_wall[i], &X_intersez, &Y_intersez))
		{
			
			#ifdef DEBUG
				cout << "\nWarning: wall crossed at: "<< X_intersez << " _ "<< Y_intersez << endl;
			#endif
			wall_crossed=true;
			
			
		}
		
		
		
		if (test_intersection(x, y, x_new, y_new, x2_wall, y1_wall[i], x1_wall, y1_wall[i], &X_intersez, &Y_intersez))
		{
			
			#ifdef DEBUG
				cout << "\nWarning: wall crossed at: "<< X_intersez << " _ "<< Y_intersez << endl;
			#endif
			wall_crossed=true;
			
			
		}
		
		#ifdef DEBUG
		cout << "" << endl;
		#endif
	}	

	
	// test: did the robot wander outside the arena?
	unsigned int arena_limit = (ARENA * N_WALLS) + 20;
	if(x_new < 0 ||x_new > arena_limit || y_new < 0 || y_new > 100){
		#ifdef DEBUG
			cout << "\nWarning: robot wandered outside the arena at - "<< x << " _ "<< y << endl;
		#endif
		wall_crossed = true;
		
	}


      if(!wall_crossed){

	x=x_new;
	y=y_new;
      
	}
      
      
      
      
      if(simulate && path!=nullptr)
	path << x << " "<< y << endl;
      
    } // end if( s == "move")
    else if(s == "rotate")
    {
      
      file >> rotate_by;
      
      if(rotate_by!=0)
	step_number++;
      direction+=rotate_by;
      
      
      #ifdef DEBUG
      cout<<"Rotate by: "<< rotate_by<< endl;
      cout <<endl<< "Direction: " << direction << endl;
      #endif
      
      
      
    }
    
    
  }
  
  {
    
    
    
    //Computing the fitness for an individual
    
    
    
    COMP1 = max_distance - sqrt(pow(x-x_goal,2)+pow(y-y_goal,2));
    
    if(max_distance - COMP1 <= 0.5) //threshold on precision
      COMP1=max_distance;
    
    if(step_number!=0)	//if ugp generated null rotations or movements
      COMP2=1.0f/step_number;
    else 
      COMP2=0;
    
    
    
    if(simulate)      
      cout<< COMP1 <<" " << COMP2 ;     
    
    
    //both the components of the fitness have the same weight
    
    COMP1 = COMP1/max_distance * 0.5;
    
    COMP2 = COMP2 * 0.5;
    
    
    //in order to avoid that fitness becomes zero or negative
    COMP1=max(0.1f,COMP1);
    COMP2=max(0.000000001f,COMP2);

    ofile<< COMP1 <<" " <<COMP2 <<endl;
    myfitness<< COMP1 <<" " <<COMP2 <<endl;
    
    
    if(simulate)
      
      cout<<" norm: " <<COMP1 <<" " << COMP2<<endl ;     
    
    
    
    
    
    
    
  }
  
  
  file.close();
  }
  
  ofile.close();
  myfitness.close();
  
  if(path.is_open())
    path.close();
  
  
  return 0;
}

bool test_intersection(float Ax, float Ay,float Bx, float By,float Cx, float Cy,float Dx, float Dy,float *X,float *Y){
  //test if two sements instersect
  //AB
  //CD
	#ifdef DEBUG
		cout << "Testing intersections : ("<< Cx << ", "<< Cy << "), ("
		<< Dx << ", "<< Dy << ")"
		<< endl;
	#endif

  float m1,m2,q1,q2;
  
  m1= (By-Ay) / (Bx-Ax)    ;
  m2= (Dy-Cy) / (Dx-Cx)    ;
  
  q1= Ay - Ax * (By-Ay)/(Bx - Ax);
  q2= Cy - Cx * (Dy-Cy)/(Dx - Cx);
  
  if(Ax==Bx){ //if the first segment is vertical
    
    *X=Ax;
    
    *Y=m2 * *X+q2 ; //intersection with other segment
    
    
    
  }
  else
    if(Cx==Dx){ //if the second segment is vertical
      
      *X=Cx;
      
      *Y=m1 * *X+q1 ;
      
      
    }
    else
    {    
      if((m1-m2)==0.0)
	return false; //they are parallel
	
	*X=(q2-q1)/(m1-m2);
	*Y=m1**X+q1;
      
    }
    
    //Since we are considering line segments we check that intersection is in the segments
    
    if(min(Ax,Bx) <= *X &&  *X <= max(Ax,Bx) &&  min(Ay,By) <= *Y && *Y <= max(Ay,By) && min(Cx,Dx) <= *X &&  *X <= max(Cx,Dx) &&  min(Cy,Dy) <= *Y && *Y <= max(Cy,Dy) )
      return true;
    else return false;//the segments don't intersect
      
      
}














