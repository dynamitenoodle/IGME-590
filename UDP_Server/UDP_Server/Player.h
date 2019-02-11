#pragma once
#include "stdafx.h"

class Player
{
public:
	Player();
	Player(char clientIp[256], int playerNum, string name, int x, int y);
	~Player();

	// Gets the player's name
	string GetName();

	// Gets the player's IP Address
	string GetIp();

	// Gets the player's current TreasureAmt
	int GetTreasureAmt();

	// Position Coords
	int positionX;
	int positionY;

	bool ColCheck(int x, int y);

	// Adds treasure to the player's total
	void AddTreasure();

private:
	char clientIp[256];
	int playerNum;
	string name;

	int treasureAmt;
};

