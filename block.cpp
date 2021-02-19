/*
-Author:		Ronald Quiroz
-Professor:		P. Allen
-Course:		COP 2001 summer2020
-File:			block.cpp
-Purpose:		implementation file for a block class that represents a rectangle game object
				with position, size and velocity.
*/
#include "block.h"


//-----------------------------------------------------------
	//	constructors
	//-----------------------------------------------------------
//default constructor
Block::Block()
{
	x = 0.0f;
	y = 0.0f;
	width = 0;
	height = 0;
	color = Black;
	velocityX = 0.0f;
	velocityY = 0.0f;
	before = nullptr;
	after = nullptr;
}

//property constructor
Block::Block(float x, float y, float width, float height, Color color,
			 float velocityX, float velocityY, Block* before, Block* after)
{
	this->x = x;
	this->y = y;
	this->width = width;
	this->height = height;
	this->color = color;
	this->velocityX = velocityX;
	this->velocityY = velocityY;
	this->before = before;
	this->after = after;
}
//copy constructor
Block::Block(Block* block)
{
	x = block->x;
	y = block->y;
	width = block->width;
	height = block->height;
	color = block->color;
	velocityX = block->velocityX;
	velocityY = block->velocityY;
	before = block->before;
	after = block->after;
}

//-----------------------------------------------------------
//	accessors(getters and setters)
//-----------------------------------------------------------
float  Block::getX() { return x; }
void   Block::setX(float value) { x = value; }
float  Block::getY(){ return y; }
void   Block::setY(float value){ y = value; }
int    Block::getWidth(){ return width; }
void   Block::setWidth(int value){ width = value; }
int    Block::getHeight(){ return height; }
void   Block::setHeight(int value){ height = value; }
Color  Block::getColor(){ return color; }
void   Block::setColor(Color value){ color = value; }
float  Block::getVelocityX(){ return velocityX; }
void   Block::setVelocityX(float value){ velocityX = value; }
float  Block::getVelocityY(){ return velocityY; }
void   Block::setVelocityY(float value){ velocityY = value; }
Block* Block::getBefore(){ return before; } 
void   Block::setBefore(Block* ptrBlock){ before = ptrBlock; }
Block* Block::getAfter(){ return after; }
void   Block::setAfter(Block* ptrBlock){ after = ptrBlock; }

//-----------------------------------------------------------
//	member methods
//-----------------------------------------------------------
//draw a single block object. has a window and a block parameter. returns nothing.
void Block::draw(OpenGL& window)
{
	float position[2]{ x, y };
	float size[2]{ width, height };
	window.DrawSprite(position, size, reinterpret_cast <const unsigned char*> (&color));

}

//determines if 2 blocks intersect one another. return a bool. takes 2 blocks.
bool Block::intersects(Block* other)
{
	return !(int(other->x) > int(this->x + this->width) ||
		int(other->x + other->width) < int(this->x) ||
		int(other->y) > int(this->y + this->height) ||
		int(other->y + other->height) < int(this->y)
		);

}

//determines if a block has velocity in either x or y coordinates. has a block parameter. and returns a bool.
bool Block::isMoving()
{
	return int(velocityX) != 0 || int(velocityY) != 0;
}

//move a block based on a set distance. paramater distances in pixels to move the snake. returns void.
void Block::move(float distance)
{
	x += velocityX * distance;
	y += velocityY * distance;
}

//link a new block to the after pointer of this block. return void. parameter a block pointer 'other'.
void Block::append(Block* other)
{
	//create a new block as copy as other block.
	Block* newTail = new Block(other);

	//attach new block behind this one.
	after = newTail;

	//make new block's foward pointer point  in this one.
	newTail->before = this;

}