/*
-Author:		Ronald Quiroz
-Professor:		P. Allen
-Course:		COP 2001 summer2020
-File:			source file Snake lab 4.
-Purpose:		main application file for the snake game, provides startup and shutdown, and the main game-loop. now added movement controls.!
*/


#include<iostream>				//console i/o
#include<stdlib.h>				//random numbers
#include<time.h>				//seed random numbers

#include "opengl.h"				//graphics library
#include "snakedefs.h"			//game definitions
#include "block.h"				//game block objects
#include "snake.h"				//the snake object


//graphics library
//-------------------------------
//global space = CONSTANTS, functions, data structures, etc.
//-------------------------------
//prototype for render method
void InitGameObjects(Block borders[], Snake& snake, Block& food);
//prototype for processinput
Direction processInput(OpenGL& window);
//prototype for update
bool update(Snake& snake, Block borders[], Block& food, float deltaTime);
void render(OpenGL& window, Block borders[], Snake snake, Block food);
bool CollisionChecks(Snake snake, Block bordersp[]);

Block NextFood(Snake snake);


int main()
{
	//initiate random number sequences
	srand(time(NULL));

	//create our graphics window for drawing.
	OpenGL window = OpenGL(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_TITTLE.c_str());
	
	//border walls
	Block borders[4];
	//the snake
	Snake snake = Snake();		//initialize with the default constructor
	//the food block
	Block food; 

	InitGameObjects(borders, snake, food);

	//frame time variables
	float deltaTime = 0.0f;
	float lastFrame = 0.0f;


	//main game loop
	bool gameOver = false;
	while (!window.isClosing() && !gameOver)
	{
		//calculate delta time
		float currentFrame = window.getTime();
		deltaTime += (currentFrame - lastFrame) * 1000;
		lastFrame = currentFrame;

		//get user input if not still waiting to process input
		if (snake.getNext() == None)
			snake.setNext(processInput(window));

		//set gameover if user chose to exit
		if (snake.getNext() == Exit)
			gameOver = true;

		//update game objects at each sub-frame interval
		while (!gameOver && deltaTime >= FPS_RATE)
		{
			gameOver = update(snake, borders, food, deltaTime);
			deltaTime -= FPS_RATE;
		}

		//draw objects
		render(window, borders, snake, food);

		window.pollEvents();
	}//end game loop

	std::cout << "Game Over!! \n press any key and enter to close..." << std::endl;

	char pause;
	std::cin >> pause;

	return 0;
	//end main
}

//initialize game objects. return nothing. parameters are borders, snake and food objects.
void InitGameObjects(Block borders[], Snake& snake, Block& food)
{
	//initialize borders
	borders[TopWall] = Block(MARGIN,
							 MARGIN,
							 WINDOW_WIDTH - 2 * MARGIN,
							 BORDER_SIZE,
							 BORDER_COLOR);

	borders[BottomWall] = Block(MARGIN,
								 WINDOW_HEIGHT - MARGIN - BORDER_SIZE,
								 WINDOW_WIDTH - 2 * MARGIN,
								 BORDER_SIZE,
								 BORDER_COLOR);

	borders[LeftWall] = Block(MARGIN,
							  MARGIN,
							  BORDER_SIZE,
							  WINDOW_HEIGHT - 2 * MARGIN,
							  BORDER_COLOR);

	borders[RightWall] = Block(WINDOW_WIDTH - MARGIN - BORDER_SIZE,
							   MARGIN,
							   BORDER_SIZE,
							   WINDOW_HEIGHT - 2 * MARGIN,
							   BORDER_COLOR);

	
	food = NextFood(snake);	


}

//process input function body which will process the movement of the block input by the user. return a direction type. and requires a window type object as parameters.
Direction processInput(OpenGL& window)
{
	Direction myDirection = None;

	GL_KEY key = window.GetKey();

	switch (key)
	{
	case GL_KEY::KEY_ESC:
	case GL_KEY::KEY_X:
		myDirection = Direction::Exit;
		break;

	case GL_KEY::KEY_W:
	case GL_KEY::KEY_UP_ARROW:
		myDirection = Direction::Up;
		break;

	case GL_KEY::KEY_S:
	case GL_KEY::KEY_DOWN_ARROW:
		myDirection = Direction::Down;
		break;

	case GL_KEY::KEY_A:
	case GL_KEY::KEY_LEFT_ARROW:
		myDirection = Direction::left;
		break;

	case GL_KEY::KEY_D:
	case GL_KEY::KEY_RIGHT_ARROW:
		myDirection = Direction::right;
	}

	return myDirection;
}


//prototype for update which change the state of the objects in the game. returns a bool if the snake hit itself of a wall. and takes a snake, a an array of blocks objects, a block as food object,  and a float object parameter.
bool update(Snake& snake, Block borders[], Block& food, float deltaTime)
{
	bool collision = false;
	//skip updates until snake starts moving
	if (!snake.isMoving())
	{
		if (snake.getNext() == None)
			return false;
		else
			//make first turn
			snake.turn();
	}

	//calculate distance traveled this update
	float distance = SNAKE_VELOCITY / deltaTime;
	snake.move(distance);

	//snake has moved full block size 
	if (snake.hasMoved())
	{
		snake.turn();

		snake.reset();

		//see if the snake ate the food
		if (snake.hitBlock(&food))
		{
			//growth
			snake.grow();

			//get a new food block 
			food = NextFood(snake);

		}
		//see if the snake hit a wall or itself
		else
		{
			collision = CollisionChecks(snake, borders);			//game over
		}

	}//end distance check

	return collision;
}//end update



//body for the render method which will allow us to draw on the window made. it does not return anything. have a openGL, a block and a snake type, an a block as food as a parameter.
void render(OpenGL& window, Block borders[], Snake snake, Block food)
{
	//Color background = Teal;
	//clear windo memory buffer
	window.clearWindow();
	

	//draw the snake's body
	snake.draw(window);
 
	//draw the food block
	food.draw(window);

	//draw the border walls
	borders[TopWall].draw(window);
	borders[BottomWall].draw(window);
	borders[LeftWall].draw(window);
	borders[RightWall].draw(window);


	//repaint screen
	window.paintWindow();
}

//checks for collisions between the snake's head and the walls and snake body. returns a bool. parameters are an snake object and a block arrays.
bool CollisionChecks(Snake snake, Block borders[])
{
	bool collide = false;
	//see if snake hit top wall
	if (snake.getCurrent() == Up && snake.hitBlock(&borders[TopWall]))
		collide = true;

	//see if sanke hit bottom wall
	else if (snake.getCurrent() == Down && snake.hitBlock(&borders[BottomWall]))
		collide = true;
	 
	//see if snake hit left wall
	else if (snake.getCurrent() == left && snake.hitBlock(&borders[LeftWall]))
		collide = true;
	
	//see if snake hit right wall
	else if (snake.getCurrent() == right && snake.hitBlock(&borders[RightWall]))
		collide = true;
	
	//see if the snake hit itself
	else
	{ 
		collide = snake.hitSelf();
	}

	return collide;
}//end collision 





//generate random location for new food. returns block. takes a snake object as a parameter.
Block NextFood(Snake snake)
{
	Block food = Block(0, 0, BLOCK_SIZE, BLOCK_SIZE, FOOD_COLOR);

	bool placed = false;
	while (!placed)
	{
		//get random (x,y) within game window
		int xCoord = rand() % (GAME_WIDTH - BLOCK_SIZE);	//random column
		int yCoord = rand() % (GAME_HEIGHT - BLOCK_SIZE);	//random row

		//move coordinates into visible screen
		xCoord += (MARGIN + BORDER_SIZE);
		yCoord += (MARGIN + BORDER_SIZE);

		food.setX(xCoord);
		food.setY(yCoord);

		placed = !snake.hitBlock(&food, true);								


	}//while not placed

	return food;
}//end NextFood