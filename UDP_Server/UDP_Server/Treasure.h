#pragma once
#include "stdafx.h"

class Treasure
{
public:
	Treasure();
	Treasure(int x, int y);
	~Treasure();

	// Checks if the position passed in matches the treasure's
	bool ColCheck(int x, int y);

	int positionX;
	int positionY;
};

