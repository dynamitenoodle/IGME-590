#pragma once
#include "stdafx.h"

class Player
{
public:
	Player(char clientIp[256], int playerNum, char* name, int x, int y);
	~Player();

	string GetName();
	string GetIp();
private:
	char clientIp[256];
	int playerNum;
	char* name;

	// Position Coords
	int positionX;
	int positionY;
};

