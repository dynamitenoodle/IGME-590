#include "Treasure.h"

Treasure::Treasure()
{
	this->positionX = -1;
	this->positionY = -1;
}

Treasure::Treasure(int x, int y)
{
	this->positionX = x;
	this->positionY = y;
}


Treasure::~Treasure()
{
}

bool Treasure::ColCheck(int x, int y)
{
	return this->positionX == x && this->positionY == y;
}

