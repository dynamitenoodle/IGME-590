#include "Player.h"

Player::Player()
{
	strcpy_s(this->clientIp, "0");
	this->playerNum = 0;
	this->name = "";

	// Position Coords
	this->positionX = -25;
	this->positionY = -25;
}

Player::Player(char clientIp[256], int playerNum, string name, int x, int y)
{
	strcpy_s(this->clientIp, clientIp);
	this->playerNum = playerNum;
	this->name = name;

	// Position Coords
	this->positionX = x;
	this->positionY = y;

	this->treasureAmt = 0;
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

int Player::GetTreasureAmt()
{
	return this->treasureAmt;
}

bool Player::ColCheck(int x, int y)
{
	return this->positionX == x && this->positionY == y;
}

void Player::AddTreasure()
{
	treasureAmt++;
}
