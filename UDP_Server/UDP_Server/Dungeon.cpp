#include "Dungeon.h"

Dungeon::Dungeon()
{
}


Dungeon::~Dungeon()
{
}

void Dungeon::DisplayDungeon()
{

}

void Dungeon::AddPlayer(char clientIp[256], char* name)
{
	Player newPlayer(clientIp, playerCount, name, 0, 0);
	playerArray[playerCount] = newPlayer;
	playerCount++;

	cout << "Added " << playerArray[playerCount - 1].GetName << " to the Dungeon" << endl;
}
