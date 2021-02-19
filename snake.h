/*
-Author:		Ronald Quiroz
-Professor:		P. Allen
-Course:		COP 2001 summer2020
-File:			snake.h
-Purpose:		header file for a snake class for a snake game where the snake 
				moves around and growths when it eats food.
*/
#ifndef SNAKE_H
#define SNAKE_H

#include "block.h"
#include "snakedefs.h"

class Snake
{
public:
	//-----------------------------------------------------------
	//	constructors
	//-----------------------------------------------------------
	Snake();			//default


	//-----------------------------------------------------------
	//	accessors
	//-----------------------------------------------------------
	Block* getHead();
	Block* getTail();
	int    getSize();
	float  getDistance();

	Direction getCurrent();
	Direction getNext();
	void	  setNext(Direction value);
	
	//-----------------------------------------------------------
	//	Member methods
	//-----------------------------------------------------------
	bool isMoving();
	void turn();
	void move(float distance);
	bool hasMoved();
	void reset();
	bool hitSelf();
	bool hitBlock(Block* block, bool all = false);
	void grow();
	void draw(OpenGL& window);

private:
	Block head;						//root of snake
	Block* tail;					//tail of the snake
	int size;						//how big the snake has grown(starts at 1)
	Direction current;				//current direction the snake is traveling
	Direction next;					// next direction user has inputed
	float distance;					//distance the snake has traveled
};






#endif
