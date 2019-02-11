#include "Dungeon.h"

Dungeon::Dungeon(int sizeX, int sizeY, int maxTreasure)
{
	this->playerArray = new Player[10]();
	this->sizeX = sizeX;
	this->sizeY = sizeY;
	int minimum = (int)floor((sizeX * sizeY) / 10);
	if (maxTreasure - minimum <= 0)
		this->treasureAmt = minimum;
	else
		this->treasureAmt = ((rand() % (maxTreasure - minimum)) + minimum);

	GenerateTreasure();
}


Dungeon::~Dungeon()
{
}

Player* Dungeon::GetPlayerArray()
{
	return playerArray;
}

int Dungeon::GetPlayerCount()
{
	return playerCount;
}

void Dungeon::AddPlayer(char clientIp[256], string name)
{
	// creates a random position thats not on a treasure
	int randX = rand() % sizeX;
	int randY = rand() % sizeX;

	// makes sure that we have a new position for the player
	while (true)
	{ 
		randX = rand() % sizeX;
		randY = rand() % sizeX;

		// checks to see that the player is in a new position is new before iterating
		bool newTreasureCheck = true;
		for (int i = 0; i < treasureAmt; i++)
		{
			if (treasures[i].ColCheck(randX, randY))
				newTreasureCheck = false;
		}

		for (int i = 0; i < playerCount; i++)
		{
			if (playerArray[i].ColCheck(randX, randY))
				newTreasureCheck = false;
		}

		if (newTreasureCheck)
			break;
	}

	Player newPlayer(clientIp, playerCount, name, randX, randY);
	playerArray[playerCount] = newPlayer;
	playerCount++;

	cout << "Added " << playerArray[playerCount - 1].GetName() << " to the Dungeon" << endl;
}

void Dungeon::RemovePlayer(string name)
{
	// finds the player to remove
	int toRemove = -1;
	for (int i = 0; i < playerCount; i++)
	{
		if (name == playerArray[i].GetName())
		{
			toRemove = i;
		}
	}

	// moves the array down
	for (int i = toRemove + 1; i < playerCount; i++)
	{
		playerArray[i - 1] = playerArray[i];
	}

	// sets the last spot to a blank player and decrements the player count
	playerArray[playerCount] = Player();
	playerCount--;

	// If we couldn't find the Player
	if (toRemove == -1)
		cout << "Couldn't find player " << name << endl;
}

Player Dungeon::GetPlayer(string name)
{
	for (int i = 0; i < playerCount; i++)
	{
		if (name == playerArray[i].GetName())
		{
			return playerArray[i];
		}
	}
	cout << "Couldn't find player " << name << endl;
	return Player();
}

string Dungeon::DisplayValues(string name)
{
	// creates an array with the display values
	string values = "\nThere appears to be ";

	values += to_string(this->treasureAmt) + " treasures left in the ";
	values += to_string(this->sizeX) + " by ";
	values += to_string(this->sizeY) + " dungeon. \nYou appear to be at the coordinates ";
	for (int i = 0; i < playerCount; i++)
	{
		if (playerArray[i].GetName() == name)
		{
			values += to_string(playerArray[i].positionX) + "-";
			values += to_string(playerArray[i].positionY) + ".";
		}
	}

	return values;
}

bool Dungeon::GetTreasure(string name, bool take)
{
	for (int i = 0; i < treasureAmt; i++)
	{
		// if we find the treasure
		if (treasures[i].ColCheck(GetPlayer(name).positionX, GetPlayer(name).positionY))
		{
			// remove it and add to the players total
			if (take)
			{
				RemoveTreasure(GetPlayer(name).positionX, GetPlayer(name).positionY);
				AddTreasure(name);
			}
			return true;
		}
	}
	return false;
}

bool Dungeon::GetTreasure(int x, int y, bool take)
{
	for (int i = 0; i < treasureAmt; i++)
	{
		// if we find the treasure
		if (treasures[i].ColCheck(x, y))
		{
			return true;
		}
	}
	return false;
}

void Dungeon::MovePlayer(string name, int x, int y)
{
	for (int i = 0; i < playerCount; i++)
	{
		if (playerArray[i].GetName() == name)
		{
			playerArray[i].positionX += x;
			playerArray[i].positionY += y;
		}
	}
}

void Dungeon::AddTreasure(string name)
{
	for (int i = 0; i < playerCount; i++)
	{
		if (playerArray[i].GetName() == name)
		{
			playerArray[i].AddTreasure();
		}
	}
}

void Dungeon::GenerateTreasure()
{
	treasures = new Treasure[treasureAmt];

	// loops to create all the treasures needed
	for (int i = 0; i < treasureAmt;)
	{
		treasures[i].positionX = rand() % sizeX;
		treasures[i].positionY = rand() % sizeY;
		
		// checks to see that the treasure is new before iterating
		bool newTreasureCheck = true;
		for (int j = 0; j < i; j++)
		{
			if (treasures[i].ColCheck(treasures[j].positionX, treasures[j].positionY) && j != i)
				newTreasureCheck = false;
		}

		if (newTreasureCheck)
			i++;
	}
}

void Dungeon::RemoveTreasure(int x, int y)
{
	// finds the player to remove
	int toRemove = -1;
	for (int i = 0; i < treasureAmt; i++)
	{
		if (treasures->ColCheck(x, y))
		{
			toRemove = i;
		}
	}

	// moves the array down
	for (int i = toRemove + 1; i < treasureAmt; i++)
	{
		treasures[i - 1] = treasures[i];
	}

	// sets the last spot to a blank player and decrements the player count
	treasures[treasureAmt] = Treasure();
	treasureAmt--;

	// If we couldn't find the Treasure
	if (toRemove == -1)
		cout << "Couldn't find treasure to remove at " << x << "-" << y << "." << endl;
}
