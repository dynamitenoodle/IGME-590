#pragma once
#include "stdafx.h"
#include "Status.h"
#include "Player.h"

class Dungeon
{
public:
	Dungeon();
	~Dungeon();

	// Displays the dungeon to the client that requests it
	void DisplayDungeon();

	// Adds a player to the dungeon
	void AddPlayer(char clientIp[256], char* name);
private:
	int playerCount;
	Player* playerArray;

};

