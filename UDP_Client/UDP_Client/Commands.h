#pragma once
#include "stdafx.h"

class Commands
{
public:
	Commands();
	~Commands();

	command connect;
	command display;
	command leave;
	command move;
	command inspect;
	command getTreasure;
	command treasureAmnt;
	command testConnect;
	command north;
	command south;
	command east;
	command west;
};

