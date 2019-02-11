#pragma once
#include "stdafx.h"
#include "Status.h"
#include "Player.h"
#include "Treasure.h"

class Dungeon
{
public:
	Dungeon(int sizeX, int sizeY, int maxTreasure);
	~Dungeon();

	// Gets the playerArray
	Player* GetPlayerArray();

	// Gets the playerCount
	int GetPlayerCount();

	// Adds a player to the dungeon
	void AddPlayer(char clientIp[256], string name);

	// Removes a player from the dungeon
	void RemovePlayer(string name);

	// Get Player by name
	Player GetPlayer(string name);

	// Creates the string of values for the display command
	string DisplayValues(string name);

	// Checks to see if there is treasure in the player's room, and adds it to their total and removes it if there is
	bool GetTreasure(string name, bool take);

	// Checks to see if there is treasure in a room
	bool GetTreasure(int x, int y, bool take);

	// Move a player
	void MovePlayer(string name, int x, int y);

private:
	int playerCount;
	Player* playerArray;
	int treasureAmt;
	int sizeX;
	int sizeY;
	Treasure* treasures;

	// Create the dungeon grid
	void GenerateTreasure();

	// removes treasure from the certain coordinates
	void RemoveTreasure(int x, int y);
};

