/*
-Author:		Ronald Quiroz
-Professor:		P. Allen
-Course:		COP 2001 summer2020
-File:			block.h
-Purpose:		header file for a block class that represents a rectangle game object
				with position, size and velocity.
*/
#ifndef BLOCK_H
#define BLOCK_H

#include "snakedefs.h"
#include "opengl.h"

class Block
{
public:
	//-----------------------------------------------------------
	//	constructors
	//-----------------------------------------------------------
	Block();	//default
	Block(float x, float y, float width, float height, Color color,
		  float velocityX = 0, float velocityY = 0,
		  Block* before = nullptr, Block* after = nullptr);
	Block(Block* block);	//copy constructor

	//-----------------------------------------------------------
	//	accessors(getters and setters)
	//-----------------------------------------------------------
	float getX();
	void setX(float value);
	float getY();
	void setY(float value);
	int getWidth();
	void setWidth(int value);
	int getHeight();
	void setHeight(int value);
	Color getColor();
	void setColor(Color value);
	float getVelocityX();
	void setVelocityX(float value);
	float getVelocityY();
	void setVelocityY(float value);
	Block* getBefore();
	void setBefore(Block* ptrBlock);
	Block* getAfter();
	void setAfter(Block* ptrBlock);

	//-----------------------------------------------------------
	//	member methods
	//-----------------------------------------------------------
	void draw(OpenGL& window);
	bool intersects(Block* other);
	bool isMoving();
	void move(float distance);
	void append(Block* other);


private:
	float x;
	float y;
	int width;
	int height;
	Color color;
	float velocityX;
	float velocityY;
	Block* before;
	Block* after;
};






#endif
