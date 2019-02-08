#include "Player.h"

Player::Player(char clientIp[256], int playerNum, char* name, int x, int y)
{
	strcpy(this->clientIp, clientIp);
	this->playerNum = playerNum;;
	this->name = name;

	// Position Coords
	this->positionX = x;
	this->positionY = y;
}

Player::~Player()
{
}

string Player::GetName()
{
	string name(this->name);
	return name;
}

string Player::GetIp()
{
	string clientIp(this->clientIp);
	return clientIp;
}
