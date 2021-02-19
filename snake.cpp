/*
-Author:		Ronald Quiroz
-Professor:		P. Allen
-Course:		COP 2001 summer2020
-File:			snake.cpp
-Purpose:		implementation file for a snake class for a snake game where the snake
				moves around and growths when it eats food.
*/
#include "snake.h"

//-----------------------------------------------------------
	//	constructors
	//-----------------------------------------------------------

//default constructor
Snake::Snake()
{
	//initializate the snake with 1 body block for the head
	head = Block(WINDOW_WIDTH / 2 + BLOCK_SIZE / 2,
				 WINDOW_HEIGHT / 2 + BLOCK_SIZE / 2,
				 BLOCK_SIZE,
				 BLOCK_SIZE,
				 SNAKE_COLOR);
			
	tail = &head;
	size = 1;
	current = None;
	next = None;
	distance = 0;
}


//-----------------------------------------------------------
//	accessors
//-----------------------------------------------------------
Block* Snake::getHead() { return &head; }
Block* Snake::getTail() { return tail;  }
int    Snake::getSize() { return size; }
float  Snake::getDistance() { return distance; }

Direction Snake::getCurrent() { return current; }
Direction Snake::getNext() { return next; }
void	  Snake::setNext(Direction value) { next = value; }

//-----------------------------------------------------------
//	Member methods
//-----------------------------------------------------------

//determines if the block has velocity in either x or y coordinates. has a block parameter. and returns a bool.
bool Snake::isMoving()
{
	return head.isMoving();
}

//turn the snake based on user input direction
void Snake::turn()
{

	switch (next)
	{
	case Up:
		if (current != Down)
		{
			head.setVelocityX(0);
			head.setVelocityY(-1);
			current = next;
		}
		break;
	case Down:
		if (current != Up)
		{
			head.setVelocityX(0);
			head.setVelocityY(1);
			current = next;
		}
		break;
	case left:
		if (current != right)
		{
			head.setVelocityX(-1);
			head.setVelocityY(0);
			current = next;
		}
		break;
	case right:
		if (current != left)
		{
			head.setVelocityX(1);
			head.setVelocityY(0);
			current = next;
		}
	}//end direction switch 

	//wait for next user input
	next = None;
 
}//end turn

//move the snake a set distance. paramater distances in pixels to move the snake. returns void.
void Snake::move(float distance)
{
	// limit the distances to full blocks only
	if (this->distance + distance > BLOCK_SIZE)
		distance = BLOCK_SIZE - this->distance;

	//set snake's total distance moved
	this->distance += distance;


	Block* part = &head;
	while(part)
	{
		//move the block
		part->move(distance);
			
		//move pointer to next block after this one
		part = part->getAfter();
	}

}//end move 

//the snake has traveled a full block size. no parameters. return a bool. true if moved a full block.
bool Snake::hasMoved()
{
	return distance >= BLOCK_SIZE;

}

//reset snake's distance and align snake's direction and alignment. returns void. no parameters.
void Snake::reset()
{
	//reset full travel property
	distance = 0;

	Block* part = tail;
	//loop from tail to head to adjust velocity
	while(part->getBefore())
	{
		//copy the x velocity from the block infront of this one
		part->setVelocityX(part->getBefore()->getVelocityX());

		//if the block is moving horizontially now, then copy the
		//vertical coordinate of the one in front of this one to
		//better align the snake blocks.
		if (part->getVelocityX())
			part->setY( part->getBefore()->getY() );

		//copy the y-velocity from the block infront of this one
		part->setVelocityY( part->getBefore()->getVelocityY() );

		//if the block is moving vertically now, then copy the
		// horizontal coordinate of the one in front of this one to 
		// better align the snake blocks. 
		if (part->getVelocityY())
			part->setX( part->getBefore()->getX() );
			
		part = part->getBefore();
	}//end loop tail to head

}

//check to see if the head of snake hit the body. return a bool. no parameters.
bool Snake::hitSelf()
{
	bool collide = false;

	Block* part = head.getAfter();
	int skip = 3;
	while (part && !collide)
	{
		//skip first 3 blocks after the head
		if (skip)
			skip--;
		else
			//check the head with current part
			collide = head.intersects(part);
		
		//move to the next part
		part = part->getAfter();
	}

	return collide;

}

//check to see if a block overlapse the head of the snake or the whole snake. returns bool. pointer to block to cheach and flag to check if all snake body or only head(false is just the head, true cheack whole snake);
bool Snake::hitBlock(Block* block, bool all)
{
	bool collide = head.intersects(block);

	//do we need to check the rest of the body
	if (all && !collide)
	{

		Block* part = head.getAfter();
		while (part && !collide)
		{
				//check the block with current part
				collide = block->intersects(part);

			//move to the next part
			part = part->getAfter();
		}

	}

	return collide;
}//end of hit block

//add a new tail onto the end of the snake. return void. no parameters.
void Snake::grow()
{

	//save off the current tail location
	tail->append(tail);

	tail = tail->getAfter();

	//adjust the new tail to after current in opposite direction
	tail->setX(tail->getX() + tail->getVelocityX() * -1 * BLOCK_SIZE);
	tail->setY(tail->getY() + tail->getVelocityY() * -1 * BLOCK_SIZE);

	//increment snake's size.
	size++;
}//end of grow method

//renders the snake's body in the graphics window. returns void. parameters window.
void Snake::draw(OpenGL& window)
{ 
	Block* part = &head;
	while (part)
	{
		//move the block
		part->draw(window);

		//move pointer to next block after this one
		part = part->getAfter();
	}

}//end draw
