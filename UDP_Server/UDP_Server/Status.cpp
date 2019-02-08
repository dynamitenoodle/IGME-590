#include "Status.h"

Status::Status()
{
	// Welcome Status
	ZeroMemory((char*)&welcome, 128);
	welcome.status = 'c';
	strcpy_s(welcome.payload, "Welcome to the dungeon");

	ZeroMemory((char*)&displayDungeon, 128);
	displayDungeon.status = 'd';
	strcpy_s(welcome.payload, "Displaying Dungeon...");
}


Status::~Status()
{
}
